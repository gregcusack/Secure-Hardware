import string
import random


N = 256

#print(''.join(random.SystemRandom().choice(
#	string.printable) for _ in range(N)))

characters = string.ascii_lowercase + string.ascii_uppercase + string.digits
print("".join(random.SystemRandom().choice(characters) for _ in range(N)))