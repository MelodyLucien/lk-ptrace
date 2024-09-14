#include <iostream>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

void GetWaitStatus(int wait_status, pid_t child_pid) {
    waitpid(child_pid, &wait_status, 0);
    if (WIFEXITED(wait_status)) {
        std::cout << "Child exited with status " << WEXITSTATUS(wait_status) << std::endl;
    } else if (WIFSIGNALED(wait_status)) {
        std::cout << "Child killed by signal " << WTERMSIG(wait_status) << std::endl;
    } else if (WIFSTOPPED(wait_status)) {
        std::cout << "Child stopped by signal " << WSTOPSIG(wait_status) <<":" << strsignal(WSTOPSIG(wait_status)) <<std::endl;
        // sned SIGCONT to continue the child process
        ptrace(PTRACE_CONT, child_pid, nullptr, nullptr);
    } else if (WIFCONTINUED(wait_status)) {
        std::cout << "Child continued" << std::endl;
    }
}

void run_debugger(pid_t child_pid) {
    int wait_status;
    struct user_regs_struct regs;

    // 附加到正在运行的子进程
    if (ptrace(PTRACE_ATTACH, child_pid, nullptr, nullptr) < 0) {
        perror("ptrace ATTACH");
        return;
    }

    // 等待子进程停止并接收 SIGTRAP 信号
    GetWaitStatus(wait_status,child_pid);

    sleep(2);

    while (true) {
        // // 获取子进程的寄存器值
        // if (ptrace(PTRACE_GETREGS, child_pid, nullptr, &regs) < 0) {
        //     perror("ptrace GETREGS");
        //     return;
        // }
        // std::cout << "RIP: " << regs.rip << std::endl;

        // // 让子进程执行一个指令
        // if (ptrace(PTRACE_SINGLESTEP, child_pid, nullptr, nullptr) < 0) {
        //     perror("ptrace SINGLESTEP");
        //     return;
        // }

        sleep(1);

        // 等待子进程停止
        GetWaitStatus(wait_status,child_pid);
        // if (WIFEXITED(wait_status)) {
        //     std::cout << "The child process has exited." << std::endl;
        //     break;
        // } else if (WIFSTOPPED(wait_status) && WSTOPSIG(wait_status) == SIGTRAP) {
        //     // 子进程因 SIGTRAP 停止，继续调试
        //     continue;
        // } else {
        //     std::cerr << "Unexpected signal: " << WSTOPSIG(wait_status) << std::endl;
        //     break;
        // }
    }
}

// 使用ncurses库，监听程序的输入，实现单步调试

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <PID of process to debug>" << std::endl;
        return 0;
    }

    pid_t child_pid = std::stoi(argv[1]);
    run_debugger(child_pid);

    return 0;
}