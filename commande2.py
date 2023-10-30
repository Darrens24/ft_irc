import pexpect
import time

child = pexpect.spawn('nc localhost 4243')
time.sleep(1)
child.sendline('PASS ha')
child.sendline('NICK F.Nietzsche')
child.sendline('USER F.Nietzsche * * F.Nietzsche')
child.sendline('JOIN #channel_philosophy')
child.sendline('TOPIC #channel_philosophy :Je_vous_enseigne_le_surhomme')
child.interact()
