import subprocess

executable_path = '/home/ugopc/ING1/42sh/42-Sh/src/42sh'

l = [ "if", "else", "elif", "fi", ";", "'", "echo", "ls", "-n", "-e" ,"-E", "-la", "#", "titi", "tata", "toto", "\n", " ", "\t" ]
import random
error = 0
number_of_test = 1000
for i in range(number_of_test):
    str = ""
    for j in range(10):
        str += l[random.randint(0, len(l) - 1)] + " "
    ref_res = subprocess.run("echo " + '"' + str + '"' + " | bash --posix", shell=True, capture_output=True, text=True)
    res = subprocess.run(executable_path +  " -c " + '"' +str + '"', shell=True, capture_output=True, text=True)
    
    if ref_res.stdout != res.stdout and res.returncode == 0 and ref_res.returncode == 0:
        error += 1
        # print("=== Error match stdout ===")
        # print("Input: " + str)
        f = open("test.txt", "a")
        f.write('"' + str + '"\n')
        # print("res: " + res.stdout)
        # print("ref_res: " + ref_res.stdout)
        f.close()

    if ref_res.returncode != res.returncode:
        error += 1
        # print("=== Error return code ===")
        # print("Input: " + str)
        # print("")
        # print("result of 42sh: " + res.stdout)
        # print("")
        # print("result of bash --posix: " + ref_res.stdout)
        # print("")
        # print("return code of 42sh: ", end="")
        # print(res.returncode)
        # print("return code of bash --posix: ", end="")
        # print(ref_res.returncode)
        # print("")
print("Accuracy: ", end="")
print((number_of_test - error) / number_of_test * 100, end="")
print("%")