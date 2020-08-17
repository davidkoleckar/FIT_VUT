#ifndef DISK_H
#define DISK_H

/**
 * Třída reprezentuje jeden kámen.
 * Kámen může nabývat dvou barev - bílá nebo černá.
 * 
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */
class Disk
{
    public:
        Disk(bool is_white);
        void turn();
        bool is_white();
    protected:
    private:
        bool isWhite;
};

#endif // DISK_H
