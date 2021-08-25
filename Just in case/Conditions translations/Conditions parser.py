f = open('Conditions translations from OpenWeatherMap.txt', 'r', encoding="utf-8")

for line in f:
    inp = line.split()
    if len(inp) == 0: continue
    if len(inp) == 1 and inp[0] != "description:":
        print('    default:  return("Unknown (" + String(id) + ")");\n  }\n #elif (LANGUAGE == ' + inp[0][:-1].upper() + ')')
        print('  switch (id) {')
    elif inp[0] != 'description:' and inp[0] != 'main:' and inp[0] != 'name:':
        print('    case ' + inp[0] + ' return("', end = '')
        for i in range(1, len(inp)):
            if i == 1: print(inp[i].title(), end = '')
            else: print(' ' + inp[i], end = '')
        print('");')
print('    default:  return("Unknown (" + String(id) + ")");\n  }')

print('\n\nPress Enter to quit')
input()