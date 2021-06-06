import sys

if (len(sys.argv)<2):
    print('expected python3 genNPuzzle n')
    sys.exit()

n = int(sys.argv[1])
n += 1
n_sqrt = (n)**(0.5)

if ((n_sqrt)%1!=0):
    print('n must be m**2 -1')
    sys.exit()

texto = 'DOMAIN tile ' + str(n) + '\n'
texto += ' '*7 + 'b'

for i in range(1,n):
    texto += ' ' + str(i)

texto += '\n\n' + str(n) + '\n\n'
texto += 'tile ' * n + '\n\n'

n_sqrt = int(n_sqrt)

#Left
for i in range(n):
    if (i%n_sqrt != n_sqrt-1):
        texto += i * '- ' + 'b X ' + (n - i - 2) * '- '
        texto += '=> '
        texto += i * '- ' + 'X b ' + (n - i - 2) * '- '
        texto += 'LABEL LEFT\n'

#Right
for i in range(n):
    if (i%n_sqrt != n_sqrt-1):
        texto += i * '- ' + 'X b ' + (n - i - 2) * '- '
        texto += '=> '
        texto += i * '- ' + 'b X ' + (n - i - 2) * '- '
        texto += 'LABEL RIGHT\n'

#Up
for i in range(n_sqrt * (n_sqrt - 1)):
    texto += i * '- ' + 'b ' + '- ' * (n_sqrt - 1) + 'X ' + '- ' * (n - i - n_sqrt -1)
    texto += '=> '
    texto += i * '- ' + 'X ' + '- ' * (n_sqrt - 1) + 'b ' + '- ' * (n - i - n_sqrt -1)
    texto += 'LABEL UP\n'

#Down
for i in range(n_sqrt * (n_sqrt - 1)):
    texto += i * '- ' + 'X ' + '- ' * (n_sqrt - 1) + 'b ' + '- ' * (n - i - n_sqrt -1)
    texto += '=> '
    texto += i * '- ' + 'b ' + '- ' * (n_sqrt - 1) + 'X ' + '- ' * (n - i - n_sqrt -1)
    texto += 'LABEL DOWN\n'

texto += '\nGOAL '
for i in range(1,n):
    texto += str(i) + ' '
texto += 'b'

print(texto)