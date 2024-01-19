import sys
import os
import getpass

yes = ['y', 'Y', 'yes', 'Yes', 'YES']
no = ['n', 'N', 'no', 'No', 'NO']

not_main = True

def get_pass_ssid():
    if not_main:
        with open('.cache/confidential.txt', 'r') as f:
            ssid = f.readline().strip()
            password = f.readline().strip()
        return password, ssid
    # ask password and ssid
    ssid = input('ssid: ')
    password = getpass.getpass('password: ')

    # hash password by wpa_passphrase
    # $ wpa_passphrase hoge fuga
    # if password is 'hoge' and ssid is 'fuga', output is below
    # network={
    #     ssid="fuga"
    #     #psk="hoge"
    #     psk=1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcd
    # }
    password = os.popen('wpa_passphrase ' + ssid + ' ' + password).readlines()
    if len(password) < 2:
        raise Exception('password is too short')
    password = map(lambda x: x.strip(), password)
    password = filter(lambda x: x.startswith('psk='), password)
    password = list(password)[0].split('=')[1]

    print('ssid: ' + ssid)
    print('hashed password: ' + password)
    
    # store to .cache/confidential.txt
    if not os.path.exists('.cache'):
        os.mkdir('.cache')

    with open('.cache/confidential.txt', 'w') as f:
        # write password and ssid
        f.write(ssid + '\n')
        f.write(password + '\n')
    return password, ssid

def make_template(password, ssid):
    # read assets/template.cpp
    with open('autogen/template.cpp', 'r') as f:
        template = f.read()

    # replace $PASS$ and $SSID$
    template = template.replace('$WIFI_SSID$', ssid)
    template = template.replace('$WIFI_PASS$', password)
    # read assets/index.html
    with open('assets/index.html', 'r') as f:
        index = f.read()
    template = template.replace('$HTML$', index)

    # overwrite src/autogen.cpp
    with open('src/autogen.cpp', 'w') as f:
        f.write(template)

def check_validfile():
    print('check_validfile')
    if os.path.exists('src/template.cpp'):
        return True
    return False

if __name__ == '__main__':
    not_main = False
    password, ssid = get_pass_ssid()
    make_template(password, ssid)
else:
    not_main = True
    try:
        password, ssid = get_pass_ssid()
        make_template(password, ssid)
    except Exception as e:
        print(e)
        print('Please run autogen.py directly according to README.md')
        exit(1)
