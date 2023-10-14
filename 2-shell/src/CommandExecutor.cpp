#include "../headers/CommandExecutor.h"

void CommandExecutor::execArgs(Command &command) {
    pid_t pid = fork();
    execute(command, pid);
    waitpid(pid, nullptr, 0);
}

void CommandExecutor::execArgsPiped(Command &command, Command &commandPiped) {
    int pipefd[2];

    if (pipe(pipefd) < 0) {
        std::cout << "Pipe could not be initialized" << std::endl;
        exit(EXIT_FAILURE);
    }

    pid_t p1 = fork();
    execute(command, p1);
    close(pipefd[1]);
    waitpid(p1, nullptr, 0);

    pid_t p2 = fork();
    execute(commandPiped, p2);
    close(pipefd[0]);
    waitpid(p2, nullptr, 0);
}

void CommandExecutor::execArgsRedirect(Command &command) {

}

void CommandExecutor::execArgsBackground(Command &command) {
    pid_t pid = fork();
    execute(command, pid);
}

bool CommandExecutor::isBackgroundTask(Command &command) {
    auto isBackground = command.getArgsCount() > 0 && strcmp(command[command.getArgsCount() - 1], "&") == 0;
    if (isBackground) {
        command[command.getArgsCount() - 1] = nullptr;
    }
    return isBackground;
}

void CommandExecutor::execute(Command &command, pid_t pid) {
    if (pid == -1) {
        std::cout << "Failed forking child.." << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0 && execvp(command[0], command.getArgs().data()) < 0) {
        std::cout << "shll: command not found.." << std::endl;
        exit(EXIT_FAILURE);
    }
}
