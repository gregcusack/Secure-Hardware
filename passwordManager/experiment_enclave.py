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
    child.sendline("c")
    child.sendline("l")
    count = 0
    passwords = {}
    add_results = []
    get_results = []
    for length in range(4, 260, 4):
        for iteration in range(ITERATIONS):
            #print("here")
            current_name = "test{}_{}".format(iteration, length)
            current_user = "user{}_{}".format(iteration, length)
            random_password = get_random_password(length)
            passwords[current_name] = random_password

            child.sendline(current_name)
            child.sendline(current_name)
            child.sendline(random_password)
            #print("here2")
            child.expect("exec_time_add: \d+.\d+")
            temp_add = float(child.after[15:])

            #print("here3")
            child.sendline(current_name)
            child.expect('(found)')
            child.expect("exec_time_get: \d+.\d+")
            temp_get = float(child.after[15:])

            add_results.append((length, temp_add))
            get_results.append((length, temp_get))
            count += 1
            print("loop count: {}".format(count))

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

