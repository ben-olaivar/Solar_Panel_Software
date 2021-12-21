#ifndef COMMAND_PACKAGE_H
#define COMMAND_PACKAGE_H

struct Command_Package {
    bool shutdown = false;
    float ohthresh;
    float ovthresh;
    float octhresh;
};

#endif