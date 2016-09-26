#ifndef BASECOMMAND_H
#define BASECOMMAND_H

class BaseCommand{
public:
    virtual ~BaseCommand(){}
    virtual void execute() = 0;
};

#endif // BASECOMMAND_H
