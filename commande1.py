import pexpect
import time

child = pexpect.spawn('nc localhost 4243')
time.sleep(1)
child.sendline('PASS ha')
child.sendline('NICK S.Hawking')
child.sendline('USER S.Hawking * * S.Hawking')
child.sendline('JOIN #channel_science')
child.sendline('TOPIC #channel_science :*bave*')
child.sendline('MODE #channel_science')

child.interact()
