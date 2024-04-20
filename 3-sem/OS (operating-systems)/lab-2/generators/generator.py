import random
import getpass

def main():
    number_of_data = 10000
    username = getpass.getuser()

    with open("/home/" + username + "/MAI_OS/2_Lab/data_files/test_data.txt", "w+") as test_data_file :
        for _ in range(number_of_data):
            test_data_file.write(
                                 str(random.randint(-(2**30), 2**30)) + " " +
                                 str(random.randint(-(2**30), 2**30)) + " " +
                                 str(random.randint(-(2**30), 2**30)) + "\n"
                                )
            

if __name__ == "__main__" :
    main()
