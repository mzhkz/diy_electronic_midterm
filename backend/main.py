import psutil
import serial
import time

stacks = []
STACK_SIZE = 5
SERIAL_CODE = "/dev/cu.usbmodem1432101"

previous_value = 0
previous_cpu_usage = 0
is_increasing = 123

ser = serial.Serial(SERIAL_CODE, 115200, timeout = 0.5)
time.sleep(2)


def get_single_value(previous_value, current_value):
    code = current_value < previous_value
    margin = previous_value * 0.05
    if  previous_value-margin <= current_value <= previous_value+1:
        return 0
    else:
        return 1 if code else 2

def main():
    global stacks, STACK_SIZE, previous_value, is_increasing
    while True:
        percent_cpu_usage = psutil.cpu_percent(0.1)
        if percent_cpu_usage <= 0.0:
            percent_cpu_usage = previous_value
        else:
            previous_value = percent_cpu_usage;

        if len(stacks) < STACK_SIZE:
            stacks.append(percent_cpu_usage)
        else:
            for i in range(STACK_SIZE):
                if i < STACK_SIZE-1:
                    stacks[i] = stacks[i+1]
            stacks[STACK_SIZE-1] = percent_cpu_usage
        current_value = sum(stacks) / STACK_SIZE
        is_increasing = get_single_value(current_value, previous_value)
        previous_value = sum(stacks) / STACK_SIZE
        print("signal: {0}, usage: {1}, adj_usage: {2}".format(
            is_increasing, 
            percent_cpu_usage,
            current_value)
        )
        print(" received -> {0}".format(ser.readline().decode('utf-8')))
        ser.write("{}:{}".format(is_increasing, int(current_value)).encode("utf-8"))

if __name__ == "__main__":
    main()
