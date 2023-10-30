import pexpect
import time

child = pexpect.spawn('nc localhost 4243')
time.sleep(1)
child.sendline('PASS ha')
child.sendline('NICK A.Turing')
child.sendline('USER A.Turing * * A.Turing')
child.sendline('JOIN #channel_programming')
child.sendline('TOPIC #channel_programming :Solving_Enigma')
child.sendline('MODE #channel_programming +i')
child.interact()
