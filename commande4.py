import pexpect
import time

child = pexpect.spawn('nc localhost 4243')
time.sleep(1)
child.sendline('PASS ha')
child.sendline('NICK user')
child.sendline('USER user * * user')
child.sendline('JOIN #chan')
child.sendline('TOPIC #chan :*bave*')
child.sendline('MODE #chan')

child.interact()
