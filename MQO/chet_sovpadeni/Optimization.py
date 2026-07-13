import os
import sys
import time
from bisect import bisect_left, bisect_right

# =====================================================================
# НАСТРОЙКИ ТЕСТИРОВАНИЯ
# =====================================================================
DATA_FOLDER = r"D:\Heavy_Data\Data_Pairs"  
HOM_EXP = 3       # Проверим на тройных (тут больше всего лишних сортировок)
WINDOW_NS = 1.5   
TICK_PS = 82.3045 

# ОПТИМИЗАЦИЯ: True - отключить лишние .sort(), False - оставить как в оригинале
REMOVE_REDUNDANT_SORTS = False 

CH1, CH2, CH3, CH4 = 0, 3, 1, 2
EXTRA_OFFSETS_PS = [
    [2700, 0, -2700, 0], [-2700, 0, 2700, 0],
    [2700, 0, 0, -2700], [-2700, 0, 0, 2700],
    [0, 2700, 0, -2700], [0, -2700, 0, 2700],
    [0, 2700, -2700, 0], [0, -2700, 2700, 0]
]
EXTRA_OFFSETS_TICKS = [[int(round(off / TICK_PS)) for off in dataset] for dataset in EXTRA_OFFSETS_PS]

# =====================================================================
# ФУНКЦИИ ПОИСКА (С ФЛАГОМ ОПТИМИЗАЦИИ СОРТИРОВКИ)
# =====================================================================

def count_coincidences(times1, times2, window_ns, tick_ps):
    """
    Считает двойные совпадения методом двух указателей за O(N + M).
    """
    window_ticks = int(window_ns * 1000 / tick_ps)
    count = 0
    idx2 = 0
    len2 = len(times2)
    
    for t1 in times1:
        # Условие левой границы окна: t2 >= t1 - window_ticks
        # Сдвигаем idx2 вперед, пока элементы меньше левой границы
        while idx2 < len2 and times2[idx2] < t1 - window_ticks:
            idx2 += 1
            
        # Теперь idx2 указывает на начало временного окна для текущего t1.
        # Запускаем внутренний указатель для подсчета совпадений внутри окна
        curr_idx2 = idx2
        upper_bound = t1 + window_ticks
        
        while curr_idx2 < len2 and times2[curr_idx2] <= upper_bound:
            count += 1
            curr_idx2 += 1
            
        # При переходе к следующему t1 мы НЕ сбрасываем idx2 в 0. 
        # Он остается на месте, так как t1 увеличивается, и левая граница окна тоже только растет.
    return count

def find_pair_coincidences(times1, times2, window_ns, tick_ps):
    """
    Возвращает список средних времён для всех пар, попавших в окно.
    Использует метод двух указателей.
    """
    window_ticks = int(window_ns * 1000 / tick_ps)
    pair_times = []
    idx2 = 0
    len2 = len(times2)
    
    for t1 in times1:
        while idx2 < len2 and times2[idx2] < t1 - window_ticks:
            idx2 += 1
            
        curr_idx2 = idx2
        upper_bound = t1 + window_ticks
        
        while curr_idx2 < len2 and times2[curr_idx2] <= upper_bound:
            pair_times.append((t1 + times2[curr_idx2]) / 2)
            curr_idx2 += 1
            
    return pair_times
def count_triple_coincidences(times1, times2, times3, window_ns, tick_ps):
    pair12 = find_pair_coincidences(times1, times2, window_ns, tick_ps)
    if not pair12:
        return 0
        
    if not REMOVE_REDUNDANT_SORTS:
        pair12.sort()
        times3_sorted = times3[:]
        times3_sorted.sort()
    else:
        # Так как times1 и times2 отсортированы, средние значения (t1+t2)/2 
        # при монотонном проходе тоже будут идти в правильном порядке.
        times3_sorted = times3

    window_ticks = int(window_ns * 1000 / tick_ps)
    count = 0
    for tp in pair12:
        left = bisect_left(times3_sorted, tp - window_ticks)
        right = bisect_right(times3_sorted, tp + window_ticks)
        count += (right - left)
    return count

def count_quadruple_coincidences(times1, times2, times3, times4, window_ns, tick_ps):
    pair12 = find_pair_coincidences(times1, times2, window_ns, tick_ps)
    pair34 = find_pair_coincidences(times3, times4, window_ns, tick_ps)
    if not pair12 or not pair34:
        return 0
        
    if not REMOVE_REDUNDANT_SORTS:
        pair12.sort()
        pair34.sort()
        
    window_ticks = int(window_ns * 1000 / tick_ps)
    count = 0
    for tp in pair12:
        left = bisect_left(pair34, tp - window_ticks)
        right = bisect_right(pair34, tp + window_ticks)
        count += (right - left)
    return count

# =====================================================================
# ПРОФАЙЛЕР ЭТАПОВ ВЫЧИСЛЕНИЙ
# =====================================================================

def run_benchmark_with_profiling(loaded_data):
    t_shift_total = 0.0
    t_search_total = 0.0
    total_coincidences = 0
    
    for name, ch_times in loaded_data:
        for extra in EXTRA_OFFSETS_TICKS:
            # Этап 1: Применение смещений (List comprehension)
            t_shift_start = time.perf_counter()
            if HOM_EXP == 1:
                t1 = [t + extra[0] for t in ch_times.get(CH1, [])]
                t2 = [t + extra[1] for t in ch_times.get(CH2, [])]
                t_shift_total += (time.perf_counter() - t_shift_start)
                
                # Этап 2: Поиск совпадений
                t_search_start = time.perf_counter()
                total_coincidences += count_coincidences(t1, t2, WINDOW_NS, TICK_PS)
                t_search_total += (time.perf_counter() - t_search_start)
                
            elif HOM_EXP == 2:
                t1 = [t + extra[0] for t in ch_times.get(CH1, [])]
                t2 = [t + extra[1] for t in ch_times.get(CH2, [])]
                t3 = [t + extra[2] for t in ch_times.get(CH3, [])]
                t4 = [t + extra[3] for t in ch_times.get(CH4, [])]
                t_shift_total += (time.perf_counter() - t_shift_start)
                
                t_search_start = time.perf_counter()
                total_coincidences += count_quadruple_coincidences(t1, t2, t3, t4, WINDOW_NS, TICK_PS)
                t_search_total += (time.perf_counter() - t_search_start)
                
            elif HOM_EXP == 3:
                t1 = [t + extra[0] for t in ch_times.get(CH1, [])]
                t2 = [t + extra[1] for t in ch_times.get(CH2, [])]
                t3 = [t + extra[2] for t in ch_times.get(CH3, [])]
                t_shift_total += (time.perf_counter() - t_shift_start)
                
                t_search_start = time.perf_counter()
                total_coincidences += count_triple_coincidences(t1, t2, t3, WINDOW_NS, TICK_PS)
                t_search_total += (time.perf_counter() - t_search_start)
                
    return t_shift_total, t_search_total, total_coincidences

# =====================================================================
# ВВОД-ВЫВОД
# =====================================================================
def load_data_from_disk():
    all_files = [os.path.join(DATA_FOLDER, f) for f in os.listdir(DATA_FOLDER) if f.endswith('.txt')]
    if not all_files:
        print(f"Нет файлов в {DATA_FOLDER}"); sys.exit(1)
        
    loaded_data = []
    for fpath in all_files:
        channel_times = {}
        with open(fpath, 'r', encoding='utf-8') as f:
            for line in f:
                parts = line.split()
                if len(parts) >= 3:
                    try:
                        channel_times.setdefault(int(parts[0]), []).append(int(parts[2]))
                    except ValueError: continue
        for ch in channel_times:
            channel_times[ch].sort()
        loaded_data.append((os.path.basename(fpath), channel_times))
    return loaded_data

def main():
    if not os.path.exists(DATA_FOLDER):
        print(f"Папка {DATA_FOLDER} не найдена."); sys.exit(1)
        
    t_io_start = time.perf_counter()
    loaded_data = load_data_from_disk()
    t_io = time.perf_counter() - t_io_start
    
    print(f"=== ПРОФАЙЛИНГ БЕНЧМАРКА (Удаление сортировок = {REMOVE_REDUNDANT_SORTS}) ===")
    print(f"Этап 1: Чтение файлов с диска и базовый парсинг: {t_io:.4f} сек")
    
    t_shift, t_search, coincs = run_benchmark_with_profiling(loaded_data)
    
    print(f"Этап 2: Применение смещений (генерация списков):  {t_shift:.4f}  сек")
    print(f"Этап 3: Алгоритм поиска совпадений:              {t_search:.4f} сек")
    print(f"----------------------------------------------------------------")
    print(f"Итого чистое время вычислений (Этап 2 + 3):       {t_shift + t_search:.4f} сек")
    print(f"Всего совпадений найдено: {coincs}")

if __name__ == "__main__":
    main()