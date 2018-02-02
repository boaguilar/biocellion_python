#!/usr/bin/env python3

#
# This module is to manage launching and monitoring jobs on 
# other systems.
#
import signal, subprocess, sys
#from util import trace_print, error_print, flush

g_alarm_count = 0
g_max_alarm_count = -1
g_alarm_timeout = 5

def alarm_handler(signal_number, stack_frame):
    global g_alarm_count
    g_alarm_count += 1
    raise IOError("Couldn't run command.")

class Command:

    def __init__(self, cmd):
        global g_alarm_timeout
        self.mCmd = cmd
        self.mExitCode = -1
        self.mLines = []
        self.mTimeout = g_alarm_timeout
        self.mUseAlarm = True
        return

    def set_use_alarm(self, value):
        self.mUseAlarm = value
        return
    
    def get_timeout(self):
        return self.mTimeout

    def set_timeout(self, timeout):
        self.mTimeout = timeout
        return

    def get_command(self):
        return self.mCmd

    def get_exit_code(self):
        return self.mExitCode

    def get_lines(self):
        return self.mLines


    def run_command_once_communicate(self):
        exit_code = 0
        
        ##util##trace_print("subprocess.Popen()")
        sp = subprocess.Popen(self.mCmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
        try:
            outputs, errors = sp.communicate(timeout=self.mTimeout)
            # error_print("sp.communicate() finished")
            # error_print(str(outputs))
            # error_print(str(errors))
            ##util##trace_print("sp.wait()")
            exit_code = sp.wait()
            ##util##trace_print("exit_code(raw): %s" % (str(exit_code),))
            if exit_code == None:
                exit_code = 0
            else:
                exit_code = (exit_code >> 8) & 0xFF
            ##util##trace_print("exit_code(baked): %s" % (str(exit_code),))
            ##util##trace_print("try end")
            
        except subprocess.TimeoutExpired:
            ##util##error_print("TimeoutExpired running command: %s" % (self.mCmd))
            sp.terminate()
            sp.kill()
            sp.wait()
            outputs, errors = sp.communicate()
            exit_code = 97
            
        except IOError as e:
            ##util##error_print("Timeout/IOError running command: %s" % (self.mCmd))
            sp.terminate()
            sp.kill()
            sp.wait()
            outputs = []
            errors = []
            exit_code = 99

        lines = []
        for l in outputs:
            if isinstance(l, str):
                l = l.decode("utf-8")  # convert byte data to string data
            else:
                ##util##error_print(str(l) + " " + self.mCmd)
                l = str(l)
            ##util##trace_print("l: " + str(l))
            lines.append(l.strip())
        ##util##trace_print("lines: %s" % ("::".join(lines),))
        
        return exit_code, lines

    def run_command_once_alarm(self):
        lines = []
        exit_code = 0
        
        ##util##trace_print("subprocess.Popen()")
        sp = subprocess.Popen(self.mCmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)

        try:
            if self.mUseAlarm:
                prev_handler = signal.signal(signal.SIGALRM, alarm_handler)
                signal.alarm(self.mTimeout)
                
            ##util##trace_print("sp.stdout")
            process = sp.stdout

            for l in process:
                l = l.decode("utf-8")  # convert byte data to string data
                ##util##trace_print("l: " + str(l))
                lines.append(l.strip())
            ##util##trace_print("sp.wait()")
            exit_code = sp.wait()
            ##util##trace_print("exit_code(raw): %s" % (str(exit_code),))
            if self.mUseAlarm:
                signal.alarm(0)
                signal.signal(signal.SIGALRM, prev_handler)
            if exit_code == None:
                exit_code = 0
            else:
                exit_code = (exit_code >> 8) & 0xFF
            ##util##trace_print("exit_code(baked): %s" % (str(exit_code),))
            ##util##trace_print("try end")

        except IOError as e:
            # clean up
            if self.mUseAlarm:
                signal.alarm(0)
                signal.signal(signal.SIGALRM, prev_handler)
            ##util##error_print("Timeout running command: %s" % (self.mCmd))
            print( "Timeout running command: %s" % (self.mCmd, ) )
            sp.terminate()
            sp.kill()
            sp.wait()
            exit_code = 99

        ##util##trace_print("lines: %s" % ("::".join(lines),))
        
        return exit_code, lines

    def run_command(self):
        lines = []
        exit_code = 0

        ##util##trace_print("command: %s" % (self.mCmd,))
        if g_max_alarm_count >= 0 and g_alarm_count >= g_max_alarm_count:
            exit_code = 98
        else:
            if self.mUseAlarm:
                exit_code, lines = self.run_command_once_alarm()
            else:
                exit_code, lines = self.run_command_once_communicate()
                
        self.mExitCode = exit_code
        self.mLines = lines
        return exit_code, lines

    
class RemoteCommand(Command):

    SSH_CMD = "/usr/bin/ssh -q -o 'PasswordAuthentication=no' -o 'StrictHostKeyChecking=no'"
    
    def __init__(self, cmd, user, host):
        Command.__init__(self, cmd)
        self.mUser = user
        self.mHost = host
        self.mCmd = "%s %s@%s '%s'" % (self.SSH_CMD, self.mUser, self.mHost, self.mCmd)
        return



def main(argv):
    print("==================================")
    c = Command("ls")
    c.run_command()
    if c.get_exit_code() != 0:
        print("Error: ", c.get_exit_code())
        return
    print(c.get_lines())

    print("==================================")
    r = RemoteCommand("ls", "cglsim", "carbon")
    r.run_command()
    if r.get_exit_code() != 0:
        print("Error: ", r.get_exit_code())
        return
    print(r.get_lines())

    print("==================================")
    return


if __name__ == "__main__":
    main(sys.argv)
