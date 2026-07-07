import subprocess
import time
import sys
import os
import numpy as np
from bisect import bisect_left, bisect_right
import matplotlib.pyplot as plt
from pylablib.devices import Newport
import scipy.optimize as opt
from scipy.signal import find_peaks
from scipy.fft import rfft, rfftfreq
import math
import ast

# ---------------------- Correlation-measurements-by-TTM8000 setup ----------------------------------------
TTM_IP = "192.168.1.60"
TTMCNVT_PATH = r"C:\Users\Dell-Nettop-2\Desktop\QT_prog\win64\ttmcnvt"
TTMCMD_PATH = r"C:\Users\Dell-Nettop-2\Desktop\QT_prog\win64\ttmcmd"

DURATION_SEC = 3  # общая длительность записи
CHUNK_DURATION_SEC = 60  # длительность одного чанка при длительной записи

# Алиса
CH1 = 0
CH2 = 3
# Боб (триггер для HOM_exp==3)
CH3 = 1
CH4 = 2  # 4-ый канал не используется в тройных совпадениях

WINDOW_NS = 1.5 # Ширина окна совпадений
TICK_PS = 82.3045 # Длительность тика в пс

NUM_ITERATIONS = 200 # Количество итераций для пикомотора
minus = 1 # Добавляем -1 если нужно двигать пикомотор в другую сторону
BASE_FILENAME = "ttm_data"
Exp_number = 0
MIN_TOTAL_COUNTS = 100000 * DURATION_SEC  # минимальная сумма счётов для проверки импульсного режима

# ------------------- Дополнительные смещения (в пикосекундах) ----------------
# Каждый внутренний список соответствует смещениям для каналов 1,2,3,4
# Если список пуст, то дополнительных смещений нет
EXTRA_OFFSETS_PS = [
    [2700, 0, -2700, 0],
    [-2700, 0, 2700, 0],
    [2700, 0, 0, -2700],
    [-2700, 0, 0, 2700],
    [0, 2700, 0, -2700],
    [0, -2700, 0, 2700],
    [0, 2700, -2700, 0],
    [0, -2700, 2700, 0]
]
# ---------------------------------------------------------------------------

# Глобальные словари для смещений
channel_offsets_ticks = {}  # базовые смещения (вводятся пользователем)
extra_offsets_ticks = []  # дополнительные смещения (преобразованные в тики)

# ---------------------- Motor setup ----------------------------------------
motor = Newport.picomotor.Picomotor8742()
axis = 1
step_size = 100 # Величина шага мотора (1 шаг около 30 нс)
n_steps = NUM_ITERATIONS

motor.setup_velocity(axis=axis, speed=200, accel=1000)
init_position = 0
motor.set_position_reference(axis, 0)


# ---------------------- Вспомогательные функции для COS-фита ----------------
def cos_model(x, offset, amplitude, omega, phase):
    """Модель: offset + amplitude * cos(omega*x + phase)"""
    return offset + amplitude * np.cos(omega * x + phase)


def estimate_omega_from_peaks(x, y):
    """Оценка угловой частоты по расстоянию между пиками (с fallback на FFT)."""
    x_uniform = np.linspace(x[0], x[-1], len(x))
    y_interp = np.interp(x_uniform, x, y)
    peaks, _ = find_peaks(y_interp, distance=len(x) // 10)
    if len(peaks) >= 2:
        periods = np.diff(x_uniform[peaks])
        avg_period = np.mean(periods)
        if avg_period > 0:
            return 2 * np.pi / avg_period
    # Fallback: использовать FFT
    y_detrend = y_interp - np.mean(y_interp)
    n = len(y_detrend)
    y_fft = rfft(y_detrend)
    freqs = rfftfreq(n, d=(x_uniform[1] - x_uniform[0]))
    amp_spec = np.abs(y_fft)
    idx_max = np.argmax(amp_spec[1:]) + 1
    return 2 * np.pi * freqs[idx_max]


# ---------------------- TTM функции ----------------------------------------
def check_ttm_connection():
    print("Проверка подключения к Correlation-measurements-by-TTM8000...")
    if os.path.exists(TTMCMD_PATH):
        try:
            result = subprocess.run([TTMCMD_PATH, "--list"], capture_output=True, text=True, timeout=5)
            if TTM_IP in result.stdout:
                print(f"Correlation-measurements-by-TTM8000 с IP {TTM_IP} обнаружен.")
                return True
        except Exception as e:
            print(f"Ошибка ttmcmd: {e}")
    param = '-n' if sys.platform.lower().startswith('win') else '-c'
    try:
        result = subprocess.run(['ping', param, '1', TTM_IP], capture_output=True, text=True, timeout=3)
        if result.returncode == 0:
            print(f"Correlation-measurements-by-TTM8000 отвечает на ping ({TTM_IP}).")
            return True
    except:
        pass
    print("Не удалось подключиться к Correlation-measurements-by-TTM8000.")
    return False


def record_chunk(iteration, duration, chunk_index):
    """Записывает один чанк во временный файл и возвращает имя файла."""
    temp_filename = f"{BASE_FILENAME}_temp_{iteration}_{chunk_index}.txt"
    print(f"      Чанк {chunk_index}: запись {duration} сек -> {temp_filename}")
    with open(temp_filename, 'w', encoding='utf-8') as f:
        proc = subprocess.Popen([TTMCNVT_PATH, "-t", TTM_IP, "-o", "t"],
                                stdout=f, stderr=subprocess.PIPE, text=True)
        time.sleep(duration)
        proc.terminate()
        proc.wait(timeout=2)
    return temp_filename


def load_times_from_file(filename):
    """Загружает времена из файла, применяя глобальные базовые смещения (channel_offsets_ticks)."""
    channel_times = {}
    with open(filename, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) < 3:
                continue
            try:
                ch = int(parts[0])
                ticks = int(parts[2])
                # Применяем базовое смещение для канала
                offset = channel_offsets_ticks.get(ch, 0)
                ticks += offset
                channel_times.setdefault(ch, []).append(ticks)
            except ValueError:
                continue
    for ch in channel_times:
        channel_times[ch].sort()
    return channel_times


def count_coincidences(times1, times2, window_ns, tick_ps):
    """Считает двойные совпадения"""
    window_ticks = int(window_ns * 1000 / tick_ps)
    count = 0
    for t1 in times1:
        left = bisect_left(times2, t1 - window_ticks)
        right = bisect_right(times2, t1 + window_ticks)
        count += (right - left)
    return count


def get_base_counts(filename, HOM_exp):
    """
    Вычисляет базовые счётчики (без дополнительных смещений) для проверки MIN_TOTAL_COUNTS.
    Возвращает кортеж в зависимости от типа эксперимента.
    """
    ch_times = load_times_from_file(filename)
    if HOM_exp == 1:
        t1 = ch_times.get(CH1, [])
        t2 = ch_times.get(CH2, [])
        cnt1, cnt2 = len(t1), len(t2)
        coinc = count_coincidences(t1, t2, WINDOW_NS, TICK_PS)
        return cnt1, cnt2, coinc
    elif HOM_exp == 2:
        t1 = ch_times.get(CH1, [])
        t2 = ch_times.get(CH2, [])
        t3 = ch_times.get(CH3, [])
        t4 = ch_times.get(CH4, [])
        cnt1, cnt2, cnt3, cnt4 = len(t1), len(t2), len(t3), len(t4)
        coinc1234 = count_quadruple_coincidences(t1, t2, t3, t4, WINDOW_NS, TICK_PS)
        return cnt1, cnt2, cnt3, cnt4, coinc1234
    elif HOM_exp == 3:
        t1 = ch_times.get(CH1, [])
        t2 = ch_times.get(CH2, [])
        t3 = ch_times.get(CH3, [])
        cnt1, cnt2, cnt3 = len(t1), len(t2), len(t3)
        coinc123 = count_triple_coincidences(t1, t2, t3, WINDOW_NS, TICK_PS)
        return cnt1, cnt2, cnt3, coinc123


def process_chunk_with_extra(filename, HOM_exp, extra_offsets_ticks):
    """
    Обрабатывает файл, применяя все наборы дополнительных смещений.
    Возвращает суммарные (по всем наборам) счётчики и совпадения для данного чанка.
    """
    # Загружаем времена с базовыми смещениями
    ch_times = load_times_from_file(filename)

    # Инициализируем сумматоры
    agg_cnt1 = 0
    agg_cnt2 = 0
    agg_cnt3 = 0
    agg_cnt4 = 0
    agg_coinc = 0  # coinc для HOM1, coinc123 для HOM3, coinc1234 для HOM2

    # Для каждого набора дополнительных смещений
    for extra in extra_offsets_ticks:
        # extra должен быть списком из 4 элементов (для каналов 1..4)
        # Применяем дополнительные смещения к временам
        # Создаём скорректированные списки (только для используемых каналов)
        if HOM_exp == 1:
            t1 = [t + extra[0] for t in ch_times.get(CH1, [])]
            t2 = [t + extra[1] for t in ch_times.get(CH2, [])]
            cnt1, cnt2 = len(t1), len(t2)
            coinc = count_coincidences(t1, t2, WINDOW_NS, TICK_PS)
            agg_cnt1 += cnt1
            agg_cnt2 += cnt2
            agg_coinc += coinc
        elif HOM_exp == 2:
            t1 = [t + extra[0] for t in ch_times.get(CH1, [])]
            t2 = [t + extra[1] for t in ch_times.get(CH2, [])]
            t3 = [t + extra[2] for t in ch_times.get(CH3, [])]
            t4 = [t + extra[3] for t in ch_times.get(CH4, [])]
            cnt1, cnt2, cnt3, cnt4 = len(t1), len(t2), len(t3), len(t4)
            coinc1234 = count_quadruple_coincidences(t1, t2, t3, t4, WINDOW_NS, TICK_PS)
            agg_cnt1 += cnt1
            agg_cnt2 += cnt2
            agg_cnt3 += cnt3
            agg_cnt4 += cnt4
            agg_coinc += coinc1234
        elif HOM_exp == 3:
            t1 = [t + extra[0] for t in ch_times.get(CH1, [])]
            t2 = [t + extra[1] for t in ch_times.get(CH2, [])]
            t3 = [t + extra[2] for t in ch_times.get(CH3, [])]
            cnt1, cnt2, cnt3 = len(t1), len(t2), len(t3)
            coinc123 = count_triple_coincidences(t1, t2, t3, WINDOW_NS, TICK_PS)
            agg_cnt1 += cnt1
            agg_cnt2 += cnt2
            agg_cnt3 += cnt3
            agg_coinc += coinc123

    # Возвращаем в зависимости от типа измерения
    if HOM_exp == 1:
        return agg_cnt1, agg_cnt2, agg_coinc
    elif HOM_exp == 2:
        return agg_cnt1, agg_cnt2, agg_cnt3, agg_cnt4, agg_coinc
    elif HOM_exp == 3:
        return agg_cnt1, agg_cnt2, agg_cnt3, agg_coinc


def record_data(iteration, HOM_exp):
    """
    Основная функция записи и обработки для одной итерации.
    Если DURATION_SEC <= CHUNK_DURATION_SEC – один файл, иначе – чанки.
    Возвращает агрегированные счётчики (с учётом всех дополнительных смещений).
    """
    if DURATION_SEC <= CHUNK_DURATION_SEC:
        # Короткая запись – один файл
        filename = f"{BASE_FILENAME}.txt"
        while True:
            print(f"  Запись {filename} ({DURATION_SEC} сек)...")
            with open(filename, 'w', encoding='utf-8') as f:
                proc = subprocess.Popen([TTMCNVT_PATH, "-t", TTM_IP, "-o", "t"],
                                        stdout=f, stderr=subprocess.PIPE, text=True)
                time.sleep(DURATION_SEC)
                proc.terminate()
                proc.wait(timeout=2)
            print(f"  Сохранено {filename}")

            # Получаем базовые счётчики для проверки
            base_counts = get_base_counts(filename, HOM_exp)
            if HOM_exp == 1:
                cnt1, cnt2, coinc = base_counts
                total_base = cnt1 + cnt2
                print(f"  (базовые) Канал {CH1}: {cnt1}, канал {CH2}: {cnt2}, совпадений: {coinc}")
            elif HOM_exp == 2:
                cnt1, cnt2, cnt3, cnt4, coinc1234 = base_counts
                total_base = cnt1 + cnt2 + cnt3 + cnt4
                print(
                    f"  (базовые) Каналы {CH1}:{cnt1}, {CH2}:{cnt2}, {CH3}:{cnt3}, {CH4}:{cnt4}, четверных: {coinc1234}")
            elif HOM_exp == 3:
                cnt1, cnt2, cnt3, coinc123 = base_counts
                total_base = cnt1 + cnt2 + cnt3
                print(f"  (базовые) Каналы {CH1}:{cnt1}, {CH2}:{cnt2}, триггер {CH3}:{cnt3}, тройных: {coinc123}")

            if total_base >= MIN_TOTAL_COUNTS:
                break
            else:
                print(f"  Суммарное число счётов = {total_base} < {MIN_TOTAL_COUNTS}.")
                print("  Лазер, вероятно, выпал из импульсного режима. Ожидание 60 секунд...")
                time.sleep(60)
                print("  Повторная запись для той же позиции...")
                # цикл повторится, перезаписывая файл

        # Если есть дополнительные смещения, обрабатываем с ними
        if extra_offsets_ticks:
            return process_chunk_with_extra(filename, HOM_exp, extra_offsets_ticks)
        else:
            # Если дополнительных смещений нет, просто возвращаем базовые счётчики
            return base_counts
    else:
        # Длинная запись – разбиваем на чанки
        print(f"  Длительная запись ({DURATION_SEC} сек) разбита на чанки по {CHUNK_DURATION_SEC} сек.")
        return record_data_long(iteration, DURATION_SEC, HOM_exp)


def record_data_long(iteration, total_duration, HOM_exp):
    """Разбивает запись на чанки, для каждого чанка применяет все наборы смещений и суммирует."""
    num_full_chunks = int(total_duration // CHUNK_DURATION_SEC)
    last_duration = total_duration - num_full_chunks * CHUNK_DURATION_SEC

    # Суммирующие переменные для всей итерации
    agg_cnt1 = 0
    agg_cnt2 = 0
    agg_cnt3 = 0
    agg_cnt4 = 0
    agg_coinc = 0  # для HOM1: coinc, HOM2: coinc1234, HOM3: coinc123

    # Обработка полных чанков
    for chunk_idx in range(num_full_chunks):
        # Цикл перезаписи чанка, если мало счётчиков
        while True:
            temp_file = record_chunk(iteration, CHUNK_DURATION_SEC, chunk_idx + 1)
            # Получаем базовые счётчики для проверки
            base_counts = get_base_counts(temp_file, HOM_exp)
            if HOM_exp == 1:
                cnt1, cnt2, coinc = base_counts
                total_base = cnt1 + cnt2
            elif HOM_exp == 2:
                cnt1, cnt2, cnt3, cnt4, coinc1234 = base_counts
                total_base = cnt1 + cnt2 + cnt3 + cnt4
            elif HOM_exp == 3:
                cnt1, cnt2, cnt3, coinc123 = base_counts
                total_base = cnt1 + cnt2 + cnt3

            if total_base >= MIN_TOTAL_COUNTS:
                # Если ок, обрабатываем чанк с дополнительными смещениями
                if extra_offsets_ticks:
                    chunk_counts = process_chunk_with_extra(temp_file, HOM_exp, extra_offsets_ticks)
                else:
                    chunk_counts = base_counts
                break
            else:
                print(f"  Суммарное число счётов = {total_base} < {MIN_TOTAL_COUNTS}.")
                print("  Лазер, вероятно, выпал из импульсного режима. Ожидание 60 секунд...")
                time.sleep(60)
                print("  Повторная запись для того же чанка...")
                # цикл повторится, перезаписывая файл

        # Суммируем результаты чанка
        if HOM_exp == 1:
            cnt1, cnt2, coinc = chunk_counts
            agg_cnt1 += cnt1
            agg_cnt2 += cnt2
            agg_coinc += coinc
        elif HOM_exp == 2:
            cnt1, cnt2, cnt3, cnt4, coinc1234 = chunk_counts
            agg_cnt1 += cnt1
            agg_cnt2 += cnt2
            agg_cnt3 += cnt3
            agg_cnt4 += cnt4
            agg_coinc += coinc1234
        elif HOM_exp == 3:
            cnt1, cnt2, cnt3, coinc123 = chunk_counts
            agg_cnt1 += cnt1
            agg_cnt2 += cnt2
            agg_cnt3 += cnt3
            agg_coinc += coinc123
        # Удаляем временный файл чанка
        os.remove(temp_file)

    # Обработка последнего неполного чанка
    if last_duration > 0:
        while True:
            temp_file = record_chunk(iteration, last_duration, num_full_chunks + 1)
            base_counts = get_base_counts(temp_file, HOM_exp)
            if HOM_exp == 1:
                cnt1, cnt2, coinc = base_counts
                total_base = cnt1 + cnt2
            elif HOM_exp == 2:
                cnt1, cnt2, cnt3, cnt4, coinc1234 = base_counts
                total_base = cnt1 + cnt2 + cnt3 + cnt4
            elif HOM_exp == 3:
                cnt1, cnt2, cnt3, coinc123 = base_counts
                total_base = cnt1 + cnt2 + cnt3

            if total_base >= MIN_TOTAL_COUNTS:
                if extra_offsets_ticks:
                    chunk_counts = process_chunk_with_extra(temp_file, HOM_exp, extra_offsets_ticks)
                else:
                    chunk_counts = base_counts
                break
            else:
                print(f"  Суммарное число счётов = {total_base} < {MIN_TOTAL_COUNTS}.")
                print("  Лазер, вероятно, выпал из импульсного режима. Ожидание 60 секунд...")
                time.sleep(60)
                print("  Повторная запись для последнего чанка...")
        # Суммируем
        if HOM_exp == 1:
            cnt1, cnt2, coinc = chunk_counts
            agg_cnt1 += cnt1
            agg_cnt2 += cnt2
            agg_coinc += coinc
        elif HOM_exp == 2:
            cnt1, cnt2, cnt3, cnt4, coinc1234 = chunk_counts
            agg_cnt1 += cnt1
            agg_cnt2 += cnt2
            agg_cnt3 += cnt3
            agg_cnt4 += cnt4
            agg_coinc += coinc1234
        elif HOM_exp == 3:
            cnt1, cnt2, cnt3, coinc123 = chunk_counts
            agg_cnt1 += cnt1
            agg_cnt2 += cnt2
            agg_cnt3 += cnt3
            agg_coinc += coinc123
        os.remove(temp_file)

    if HOM_exp == 1:
        return agg_cnt1, agg_cnt2, agg_coinc
    elif HOM_exp == 2:
        return agg_cnt1, agg_cnt2, agg_cnt3, agg_cnt4, agg_coinc
    elif HOM_exp == 3:
        return agg_cnt1, agg_cnt2, agg_cnt3, agg_coinc


def find_pair_coincidences(times1, times2, window_ns, tick_ps):
    """
    Возвращает список средних времён для всех пар (t1, t2), попавших в окно.
    """
    window_ticks = int(window_ns * 1000 / tick_ps)
    pair_times = []
    times2_sorted = times2[:]
    times2_sorted.sort()
    for t1 in times1:
        left = bisect_left(times2_sorted, t1 - window_ticks)
        right = bisect_right(times2_sorted, t1 + window_ticks)
        for idx in range(left, right):
            t2 = times2_sorted[idx]
            pair_times.append((t1 + t2) / 2)
    return pair_times


def count_triple_coincidences(times1, times2, times3, window_ns, tick_ps):
    """
    Возвращает количество тройных совпадений:
    - сначала ищем двойные совпадения между times1 и times2,
    - затем для каждого среднего времени пары ищем событие в times3 в окне.
    """
    pair12 = find_pair_coincidences(times1, times2, window_ns, tick_ps)
    if not pair12:
        return 0
    pair12.sort()
    times3_sorted = times3[:]
    times3_sorted.sort()
    window_ticks = int(window_ns * 1000 / tick_ps)
    count = 0
    for tp in pair12:
        left = bisect_left(times3_sorted, tp - window_ticks)
        right = bisect_right(times3_sorted, tp + window_ticks)
        count += (right - left)
    return count


def count_quadruple_coincidences(times1, times2, times3, times4, window_ns, tick_ps):
    """
    Возвращает количество четверных совпадений (1-2 и 3-4, и затем совпадение пар).
    """
    pair12 = find_pair_coincidences(times1, times2, window_ns, tick_ps)
    pair34 = find_pair_coincidences(times3, times4, window_ns, tick_ps)
    if not pair12 or not pair34:
        return 0
    pair12.sort()
    pair34.sort()
    window_ticks = int(window_ns * 1000 / tick_ps)
    count = 0
    for tp in pair12:
        left = bisect_left(pair34, tp - window_ticks)
        right = bisect_right(pair34, tp + window_ticks)
        count += (right - left)
    return count


# ---------------------- MAIN -----------------------------------------------
def main():
    global channel_offsets_ticks, extra_offsets_ticks

    print("=== МНОГОКРАТНЫЕ ИЗМЕРЕНИЯ Correlation-measurements-by-TTM8000 ===")
    if not check_ttm_connection():
        sys.exit(1)

    HOM_exp = input("Эксперимент: 1 - HOM (двойные), 2 - HOM с триггером для каждой пары (четверные), 3 - HOM с триггером (тройные с триггерныем каналом CH3): ").strip()
    HOM_exp = int(HOM_exp) if HOM_exp else 1

    if HOM_exp == 1:
        print(f"Итераций: {NUM_ITERATIONS}, запись {DURATION_SEC} сек, каналы {CH1} и {CH2}, окно {WINDOW_NS} нс")
        used_channels = [CH1, CH2]
    elif HOM_exp == 2:
        print(
            f"Итераций: {NUM_ITERATIONS}, запись {DURATION_SEC} сек, каналы Алисы {CH1} и {CH2}, каналы Боба {CH3} и {CH4}, окно {WINDOW_NS} нс")
        used_channels = [CH1, CH2, CH3, CH4]
    elif HOM_exp == 3:
        print(
            f"Итераций: {NUM_ITERATIONS}, запись {DURATION_SEC} сек, каналы Алисы {CH1} и {CH2}, триггерный канал {CH3}, окно {WINDOW_NS} нс")
        used_channels = [CH1, CH2, CH3]

    # Запрос базовых смещений
    print("\nВведите временные смещения (offset) для каналов (в пикосекундах).")
    print("Положительное значение сдвигает события вперёд (увеличивает время), отрицательное – назад.")
    for ch in used_channels:
        prompt = f"  Смещение для канала {ch} (пс) [0]: "
        offset_ps = input(prompt).strip()
        if offset_ps == "":
            offset_ps = 0
        else:
            offset_ps = float(offset_ps)
        offset_ticks = int(round(offset_ps / TICK_PS))
        channel_offsets_ticks[ch] = offset_ticks
        print(f"    Канал {ch}: смещение {offset_ps} пс -> {offset_ticks} тиков")

    # Преобразование дополнительных смещений в тики
    extra_offsets_ticks = []
    if EXTRA_OFFSETS_PS:
        print(f"\nОбнаружены дополнительные смещения: {len(EXTRA_OFFSETS_PS)} наборов.")
        for i, offsets_ps in enumerate(EXTRA_OFFSETS_PS):
            offsets_ticks = [int(round(off_ps / TICK_PS)) for off_ps in offsets_ps]
            extra_offsets_ticks.append(offsets_ticks)
            print(f"  Набор {i + 1}: {offsets_ps} пс -> {offsets_ticks} тиков")
    else:
        print("\nДополнительные смещения не заданы.")

    Exp_number = input("Введите номер эксперимента [0]: ").strip()
    Exp_number = int(Exp_number) if Exp_number else 0

    Sinc_fit = input("Выполнить фит провала HOM функцией sinc (0 - нет, 1 - да) [0]: ").strip()
    Sinc_fit = int(Sinc_fit) if Sinc_fit else 0

    Cos_fit = input("Выполнить фит вероятности совпадения функцией cos (0 - нет, 1 - да) [0]: ").strip()
    Cos_fit = int(Cos_fit) if Cos_fit else 0

    start_total = time.time()

    probabilities = []
    current_positions = []
    iterations = list(range(1, NUM_ITERATIONS + 1))

    for i in iterations:
        print(f"\n--- Итерация {i} ---")

        # Получаем агрегированные счётчики для текущей итерации (уже с учётом всех смещений)
        if HOM_exp == 1:
            cnt1, cnt2, coinc = record_data(i, HOM_exp)
            total_counts = cnt1 + cnt2
            print(f"  (итого) Канал {CH1}: {cnt1}, канал {CH2}: {cnt2}, совпадений: {coinc}")
            prob = coinc / total_counts if total_counts > 0 else 0
        elif HOM_exp == 2:
            cnt1, cnt2, cnt3, cnt4, coinc1234 = record_data(i, HOM_exp)
            total_counts = cnt1 + cnt2 + cnt3 + cnt4
            print(f"  (итого) Каналы {CH1}:{cnt1}, {CH2}:{cnt2}, {CH3}:{cnt3}, {CH4}:{cnt4}, четверных: {coinc1234}")
            prob = coinc1234 / total_counts if total_counts > 0 else 0
        elif HOM_exp == 3:
            cnt1, cnt2, cnt3, coinc123 = record_data(i, HOM_exp)
            total_counts = cnt1 + cnt2 + cnt3
            print(f"  (итого) Каналы {CH1}:{cnt1}, {CH2}:{cnt2}, триггер {CH3}:{cnt3}, тройных: {coinc123}")
            prob = coinc123 / total_counts if total_counts > 0 else 0

        probabilities.append(prob)
        print(f"  Вероятность: {prob:.6f}")

        # Движение мотора
        target_position = minus * i * step_size
        motor.move_to(axis, target_position)
        motor.wait_move(axis)
        current_pos = motor.get_position(axis)
        current_positions.append(current_pos)

    end_total = time.time()
    total_time = end_total - start_total
    print(f"\nОбщее затраченное время: {total_time:.2f} секунд")

    # Переводим позиции в микрометры (1 шаг = 30 нм = 0.03 мкм)
    positions_um = 0.03 * np.array(current_positions)
    x_data = positions_um * 2

    # Определение модели для sinc-фита
    def homb_dip(x, C, A, w, x0):
        """f(x) = C * (1 - A * sinc((x-x0)/w))"""
        with np.errstate(divide='ignore', invalid='ignore'):
            u = (x - x0) / w
            sinc_u = np.sin(u) / u
            sinc_u = np.where(np.abs(u) < 1e-12, 1.0, sinc_u)
        return C * (1 - A * sinc_u)

    # --------------------- SINC FIT ---------------------
    C_fit = A_fit = w_fit = x0_fit = None
    fwhm = None
    x_left_root = x_right_root = None

    if Sinc_fit == 1:
        C0 = np.max(probabilities)
        min_idx = np.argmin(probabilities)
        x0_guess = x_data[min_idx]
        min_val = probabilities[min_idx]
        A0 = 1.0 - min_val / C0 if C0 > 0 else 0.5
        half_depth = (C0 + min_val) / 2
        left_idx = None
        right_idx = None
        for j in range(min_idx, -1, -1):
            if probabilities[j] >= half_depth:
                left_idx = j
                break
        for j in range(min_idx, len(probabilities)):
            if probabilities[j] >= half_depth:
                right_idx = j
                break
        if left_idx is not None and right_idx is not None and left_idx != right_idx:
            w_guess = (x_data[right_idx] - x_data[left_idx]) / 2.0
        else:
            w_guess = (np.max(x_data) - np.min(x_data)) / 4
        w_guess = max(w_guess, 1e-3)

        print(
            f"\nНачальные оценки для sinc-фита: C0={C0:.4f}, A0={A0:.4f}, w0={w_guess:.2f} мкм, x0={x0_guess:.2f} мкм")

        try:
            popt, pcov = opt.curve_fit(homb_dip, x_data, probabilities,
                                       p0=[C0, A0, w_guess, x0_guess],
                                       bounds=([0, 0, 1e-3, -np.inf], [1, 1, np.inf, np.inf]))
            C_fit, A_fit, w_fit, x0_fit = popt
            perr = np.sqrt(np.diag(pcov)) if pcov is not None else [0, 0, 0, 0]

            half_max_level = C_fit * (1 - A_fit / 2)

            def eq(x):
                return homb_dip(x, C_fit, A_fit, w_fit, x0_fit) - half_max_level

            try:
                x_left_root = opt.fsolve(eq, x0_fit - w_fit)[0]
                x_right_root = opt.fsolve(eq, x0_fit + w_fit)[0]
                fwhm = abs(x_right_root - x_left_root)
            except:
                fwhm = None
                x_left_root = x_right_root = None

            print("\n=== РЕЗУЛЬТАТЫ SINC-ФИТА ===")
            print(f"C = {C_fit:.4f} ± {perr[0]:.4f}")
            print(f"A = {A_fit:.4f} ± {perr[1]:.4f}")
            print(f"w = {w_fit:.2f} мкм ± {perr[2]:.2f}")
            print(f"x0 = {x0_fit:.2f} мкм ± {perr[3]:.2f}")
            visibility_fit = A_fit / (2.0 - A_fit)
            print(f"Видность (из фита): {visibility_fit:.4f}")
            if fwhm is not None:
                print(f"Ширина на полувысоте (FWHM) = {fwhm:.2f} мкм")
            else:
                print("Не удалось вычислить FWHM")
        except Exception as e:
            print(f"Ошибка sinc-фита: {e}")

    # --------------------- COS FIT ---------------------
    cos_fit_params = None
    cos_visibility = None

    if Cos_fit == 1:
        offset_guess = np.mean(probabilities)
        amplitude_guess = (np.max(probabilities) - np.min(probabilities)) / 2.0
        omega_guess = estimate_omega_from_peaks(x_data, probabilities)
        phase_guess = 0.0

        print(f"\nНачальные оценки для cos-фита: offset={offset_guess:.4f}, amplitude={amplitude_guess:.4f}, "
              f"omega={omega_guess:.4f}, phase={phase_guess:.4f}")

        try:
            popt, pcov = opt.curve_fit(cos_model, x_data, probabilities,
                                       p0=[offset_guess, amplitude_guess, omega_guess, phase_guess],
                                       bounds=([-np.inf, 0, 1e-6, -np.inf], [np.inf, np.inf, np.inf, np.inf]))
            offset_cos, amp_cos, omega_cos, phase_cos = popt
            perr = np.sqrt(np.diag(pcov)) if pcov is not None else [0, 0, 0, 0]

            visibility_cos = amp_cos / offset_cos if offset_cos != 0 else np.nan

            print("\n=== РЕЗУЛЬТАТЫ COS-ФИТА ===")
            print(f"offset (смещение) = {offset_cos:.4f} ± {perr[0]:.4f}")
            print(f"amplitude (амплитуда) = {amp_cos:.4f} ± {perr[1]:.4f}")
            print(f"omega (угл. частота) = {omega_cos:.4f} ± {perr[2]:.4f} рад/мкм")
            print(f"phase (фаза) = {phase_cos:.4f} ± {perr[3]:.4f} рад")
            print(f"Видность = {visibility_cos:.4f}")

            cos_fit_params = (offset_cos, amp_cos, omega_cos, phase_cos)
            cos_visibility = visibility_cos
        except Exception as e:
            print(f"Ошибка cos-фита: {e}")

    # --------------------- ПОСТРОЕНИЕ ГРАФИКА ---------------------
    plt.figure(figsize=(10, 6))
    plt.plot(x_data, probabilities, 'o', label='Эксперимент', markersize=6, color='blue')

    x_smooth = np.linspace(min(x_data), max(x_data), 1000)

    if Sinc_fit == 1 and C_fit is not None:
        y_sinc = homb_dip(x_smooth, C_fit, A_fit, w_fit, x0_fit)
        plt.plot(x_smooth, y_sinc, '-', color='red',
                 label=f'sinc-фит: C={C_fit:.3f}, A={A_fit:.3f}, w={w_fit:.1f} мкм')
        plt.axvline(x=x0_fit, color='gray', linestyle='--', alpha=0.5, label=f'x0 = {x0_fit:.1f} мкм')
        if fwhm is not None and x_left_root is not None and x_right_root is not None:
            half_level = C_fit * (1 - A_fit / 2)
            plt.axhline(y=half_level, color='green', linestyle=':', alpha=0.7, label=f'Полувысота = {half_level:.3f}')
            plt.axvline(x=x_left_root, color='orange', linestyle=':', alpha=0.7)
            plt.axvline(x=x_right_root, color='orange', linestyle=':', alpha=0.7)

    if Cos_fit == 1 and cos_fit_params is not None:
        offset_cos, amp_cos, omega_cos, phase_cos = cos_fit_params
        y_cos = cos_model(x_smooth, offset_cos, amp_cos, omega_cos, phase_cos)
        plt.plot(x_smooth, y_cos, '-', color='purple',
                 label=f'cos-фит: offset={offset_cos:.3f}, amp={amp_cos:.3f}')
        plt.axhline(y=offset_cos, color='purple', linestyle='--', alpha=0.5, label=f'offset = {offset_cos:.3f}')
        if cos_visibility is not None:
            plt.text(0.05, 0.85, f'COS видность = {cos_visibility:.4f}', transform=plt.gca().transAxes,
                     fontsize=9, verticalalignment='top', bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))

    plt.xlabel('Задержка, мкм', fontsize=12)
    plt.ylabel('Вероятность совпадений', fontsize=12)
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig("probability_plot.png", dpi=150)
    plt.show()

    # Сохранение данных
    if probabilities:
        filename = f"Prob_data_{Exp_number}.txt"
        with open(filename, 'w', encoding='utf-8') as f:
            for idx in range(len(x_data)):
                f.write(f"{x_data[idx]}   {probabilities[idx]}\n")
        print(f"Файл {filename} сохранен")

    print("Возвращаем мотор в исходное положение")
    motor.move_to(axis, init_position)
    motor.wait_move(axis)
    motor.close()


if __name__ == "__main__":
    main()