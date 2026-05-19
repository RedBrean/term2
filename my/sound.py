import numpy as np
import sounddevice as sd
import matplotlib.pyplot as plt
import queue
import threading

# =====================
# Configuration
# =====================
RATE = 44100
CHUNK = 2048
CHANNELS = 1

# =====================
# Thread-safe queue
# =====================
audio_queue = queue.Queue()

# =====================
# Window function (Hann)
# =====================
window = np.hanning(CHUNK)

# =====================
# Audio callback (FAST!)
# =====================
def audio_callback(indata, frames, time, status):
    if status:
        print(status)
    # Only copy data, nothing else
    audio_queue.put(indata[:, 0].copy())

# =====================
# Plot setup
# =====================
plt.ion()
fig, ax = plt.subplots()

freqs = np.fft.rfftfreq(CHUNK, 1 / RATE)
line, = ax.plot(freqs, np.zeros_like(freqs))

ax.set_xscale('log')
ax.set_xlim(20, RATE // 2)
ax.set_ylim(-120, 0)
ax.set_xlabel('Frequency (Hz)')
ax.set_ylabel('Amplitude (dB)')
ax.set_title('Real-time Audio Spectrum')

# =====================
# Processing loop
# =====================
def process_audio():
    while True:
        try:
            data = audio_queue.get(timeout=1)
        except queue.Empty:
            continue

        # Apply window
        data = data * window

        # FFT
        fft = np.fft.rfft(data)
        magnitude = np.abs(fft)

        # Convert to dB (avoid log(0))
        magnitude_db = 20 * np.log10(magnitude + 1e-10)

        # Update plot
        line.set_ydata(magnitude_db)
        fig.canvas.draw()
        fig.canvas.flush_events()

# =====================
# Main
# =====================
def main():
    stream = sd.InputStream(
        channels=CHANNELS,
        samplerate=RATE,
        blocksize=CHUNK,
        callback=audio_callback,
        latency='high'
    )

    with stream:
        print("Running... Press Ctrl+C to stop")

        try:
            process_audio()
        except KeyboardInterrupt:
            print("Stopping...")

if __name__ == '__main__':
    main()