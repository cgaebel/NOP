f = open('svchost.exe', 'rb')
fileContents = f.read()
f.close()

lastOffset = -1

offsets = []

while True:
    lastOffset += 1
    lastOffset = fileContents.find(b'\xC3\xCC\xCC\xCC\xCC', lastOffset)
    
    if lastOffset != -1:
        offsets.append(lastOffset)
    else:
        break

outputFile = open('output.txt', 'w')

for o in offsets:
    print("\tPatching::PatchUnconditionalJump(", hex(o + 0x00400000), ", RETN);\t// AUTOMATICALLY GENERATED", sep='', file=outputFile)

print("\nLength: ", len(offsets), file=outputFile)
    
outputFile.close()
