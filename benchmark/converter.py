def generate(fileName):
	with open(fileName) as f:
		print '{'
		for line in f:
			if line.strip():
				numbers = line.split()
				print '{',
				for num in numbers[:-1]:
					if num == '-1':
						print '%4s' % '0,',
					else:
						print '%4s' % (str(int('0x' + num.lower(), 0) + 1) + ','),
				
				if numbers[-1] == '-1':
					print '%4s' % '0',
				else:
					print '%4s' % str(int('0x' + numbers[-1].lower(), 0) + 1),
				print '},'
		print '}'

if __name__ == "__main__":
	generate('hexa-20.txt')