import base64

c = "Y29uZGl0aW9uID0gKHNbMF0gPT0gZGlyKFtdKVs0MV1bMl0pIGFuZCAoc1s5XSA9PSBjaHIoZXZhbChzdHIodHlwZSgwKSlbNzoxMF0rJygnK3N0cihjaHIoNTQpK2Nocig1MykrJyknKSkpKSBhbmQgKHNbMl0gPT0gY2hyKG9yZChkaXIoW10pWzM2XVsxXSktMzIpKSBhbmQgKHNbMTJdID09IGNocihvcmQoW10uX19zdHJfXygpWzBdKS8yICsgMykpIGFuZCAoc1sxXSA9PSBjaHIoaW50KHN0cihldmFsKGRpcih0eXBlKDB4OTEyMykpWzU4XVs0OjddKSgnMScpKSozKSkpIGFuZCAoc1s3XSA9PSBjaHIoKGludChzWzRdKSo1KSoob3JkKHNbMl0pLzQtMSkpKSBhbmQgKHNbNV0gPT0gY2hyKG9yZCgnYScuY2FwaXRhbGl6ZSgpKStldmFsKCIyIitjaHIob3JkKCdeJykvMi01KSoyKyI1IikpKSBhbmQgKHNbNF0gPT0gc3RyKG9yZChkaXIoMC4wKVsxXVsxXSkvNSAtIG9yZCgnJCcpLzIpKSBhbmQgKHNbM10gPT0gY2hyKGludCgnMScrc3RyKG9yZCgnRycpKVs6Oi0xXSkpKSBhbmQgKHNbMTFdID09IHN0cih0eXBlKHt9KSlbOF0pIGFuZCAoc1s2XSA9PSBjaHIoaW50KGNocig3KioyKStzdHIob3JkKCcpJykpWzo6LTFdKSkpIGFuZCAoc1s4XSA9PSBkaXIoW10pWzE5XVsyXSkgYW5kICggc1sxMF0gPT0gc1swXS5jYXBpdGFsaXplKCkgKQ=="

s = raw_input()

condition = False

exec(base64.b64decode(c))

if condition is True:
    print "DSMCTF{"+s+"} is the flag!"
else:
    print "Wrong!"