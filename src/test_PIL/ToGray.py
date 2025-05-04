from PIL import Image
import sys, os

externalArgs = list(sys.argv[1:])
print(externalArgs)

def ToGray(photoPath):
    print(photoPath)
    img = Image.open(photoPath)
    img_gray = img.convert('L')
    dirPath = photoPath[:-4]
    os.makedirs(dirPath, exist_ok=True)
    destPath = os.path.join(dirPath, photoPath[:-4] + "_gray" + photoPath[-4:])
    img_gray.save(destPath)
    print(f"{photoPath} -> {destPath}")

#map(ToGray, externalArgs)
for photoPath in externalArgs:
    ToGray(photoPath)
