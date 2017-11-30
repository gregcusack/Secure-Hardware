#!/usr/bin/python3

import pexpect
import os
import time
import hashlib
import random
import string
import datetime

ITERATIONS=25

def get_random_password(length):
    characters = string.ascii_lowercase + string.ascii_uppercase + string.digits
    password = "".join(random.SystemRandom().choice(characters) for _ in range(length))
    return password

if __name__ == "__main__":
    if os.path.exists("test.dat"):
        os.remove("test.dat")
    child = pexpect.spawn("./password_manager")
    #child.expect("Program hash and load finished", timeout=5)
    #print(child.before)
    child.expect("Create account or login\? \(C/L\): ")
    child.sendline("c")
    #child.expect("Enter Master Password: ", timeout=2)
    #master_password = get_random_password(255)
    #child.sendline(master_password)
    child.expect("Create account or login\? \(C/L\): ", timeout=2)
    child.sendline("l")
    #child.expect("Enter master password: ", timeout=2)
#    child.sendline("test")
    #child.sendline(master_password)
    #print(child.before)
    count = 0
    passwords = {}
    add_results = []
    get_results = []
    for length in range(12, 100, 4):
        for iteration in range(ITERATIONS):
            #print("here");
            #child.expect("Add web account credentials or get web account credentials \(a/g\): ")
            #child.expect("Add or get web: ")
            #print(child.before)
            current_name = "test{}_{}".format(iteration, length)
            current_user = "user{}_{}".format(iteration, length)
            print("here1")
            #current_name = current_name[:-2]
            #current_user = current_user[:-2]
            random_password = get_random_password(length)
            passwords[current_name] = random_password
            print(current_name);
            print(current_user);
            print(random_password);

            child.sendline(current_name)
            child.sendline(current_name)
            child.sendline(random_password)
            print("here2")
            child.expect("exec_time_add: ")#\d+.\d+")
            #add_results.append((length, float(child.after[15:])))
            child.expect("(success)|(error)");
            print("here3")
            child.sendline(current_name)
            child.expect("exec_time_get: ")#\d+.\d+")
            print("here4")
           # get_results.append((length, float(child.after[15:])))
            count += 1

            print("loop count: {}".format(count))
            child.expect('(found)|(not found)')
            #print(child.before)
            #print(child.after)
            #get_results.append((length, get_end - get_start))
            #count += 1
            #print("count: {}".format(count))
            """
            #print(current_name);
            #print(current_user);
            #print(random_password);
            #print("here1");
            #test adding password
            #add_start = time.time()
            #child.sendline("a")
            #child.expect("Enter website name: ")
            add_start = time.time()
            #print("here2");
            child.sendline(current_name)
            #print("here3");
            #child.expect("Enter website username: ")
            child.sendline(current_name)
            #print("here4");
            #child.expect("Enter website password: ")
            child.sendline(random_password)
            #print("here5");
            add_end = time.time()
            #add_data = add_end - add_start
            add_results.append((length, add_end - add_start))
            child.expect("(success)|(error)");
            #print("here6");
#            print(child.before)
            #print("here2");
            # test getting password
            #child.expect("Add web account credentials or get web account credentials \(a/g\): ")
            #child.expect("Add or get web: ")
            #print("here3")
            #print("here4")
            #child.expect("Enter website name to get credentials: ")
            #print("here5");
            get_start = time.time()
            #print("here6");
            child.sendline(current_name)
            #print("here7");
            #child.expect('(foasdund)|(asdasdasd)')
            #child.expect('(found)|(not found)')
            #child.expect(random_password)
            #print("here8");
#            print(child.after)
            get_end = time.time()
            #get_data = get_end - get_start
            get_results.append((length, get_end - get_start))
            count += 1
            print("count: {}".format(count))
            #print("Fetched creds:\b")
            #print(child.before)
            #print(child.after)
            #print("Length: {}, add_results: {}, get_results: {}".format(length, add_data, get_data))
            """
#    child.expect("Add web account credentials or get web account credentials \(a/g\): ", timeout=2)
#    child.sendline("g")
#    child.expect("Enter website name to get credentials: ")
#    child.sendline("test")
#    print(child.before)
#    child.interact()

    try:
        os.makedirs("data/")
    except FileExistsError:
        pass
    datafile = "data/password_manager_microblaze_test_{}.csv".format(datetime.datetime.now().isoformat())
    with open(datafile, 'w+') as data_out:
        data_out.write("DATA_LENGTH,WRITE_TIME,READ_TIME\n")
        for i in range(len(add_results)):
            add_length, add_time = add_results[i]
            get_length, get_time = get_results[i]
            data_out.write("{},{},{}\n".format(add_length, add_time, get_time))


