import sys
import json
import serial
import array

var_types = {
    'NUL': '',
    'U08': 'B',
    'U16': 'H',
    'U32': 'I',
    'U64': 'Q',
    'I08': 'b',
    'I16': 'h',
    'I32': 'i',
    'I64': 'q',
    'F32': 'f',
    'D64': 'd',
}

build_id = []
log_lines = []

def find_log_str(ls):
    for ln in log_lines:
        if ln['log'] == ls:
            return ln
    return None
    
def process_artifact():
    global build_id
    global log_lines
    with open(sys.argv[1], 'rb') as log_data:
        build_id = log_data.readline()[:-1]
        print('ARTIFACT BUILD_ID: ', end='')
        print(build_id.hex())
        for line in log_data.read().decode('ascii').split('\0'):
            if len(line) > 0:
                log = json.loads(line)
                log_lines.append(log)

def get_hdr():
    # Read bytes till we find header to keep alignment
    b = ser.read(1)
    if int.from_bytes(b) != 0xE0:
        return False

    b = ser.read(1)
    if int.from_bytes(b) != 0x07:
        return False
    
    return True

def get_log():    
    b = ser.read(2)
    d = int.from_bytes(b, byteorder='little')

    l = find_log_str(d)
    if l is not None:
        print('<' + l['fil'] + ':' + str(l['lin']) + '>\t' + '[' + l['lvl'] + '] ' + l['msg'], end=" ")
        if l['len'] != 0 and l['typ'] != 'NUL':
            vb = ser.read(l['len'])
            va = array.array(var_types[l['typ']], vb)

            if d == 0:
                print(vb.hex(), end = " ")
                if build_id == vb:
                    print("<-- MATCH", end = " ")
                else:
                    print("<-- WRONG", end = " ")
            else:
                print('(' + l['typ'] + 'x' + str(len(va)) + ')', end=" ")
                print(list(va), end=" ")
            
        print()

process_artifact()
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
while True:
    if get_hdr():
        get_log()