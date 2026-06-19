from pathlib import Path
import numpy as np
from PIL import Image

folder = Path('.')
out = folder / 'no noise'
out.mkdir(exist_ok=True)

for p in folder.glob('*.png'):
    if p.stem.endswith('-NOISE'):
        continue

    noise = p.with_name(f'{p.stem}-NOISE.png')

    img = np.asarray(Image.open(p), dtype=float)
    bg = np.asarray(Image.open(noise), dtype=float)

    res = np.clip(img - bg, 0, 255).astype(np.uint8)
    Image.fromarray(res).save(out / p.name)
