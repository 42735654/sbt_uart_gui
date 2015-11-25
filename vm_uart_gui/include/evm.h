#ifndef EVM_H
#define EVM_H

#include <QByteArray>
#include "type_def.h"
#define BEFORE_MAIN_FLAG_STR "THIS_IS_USED_TO_CALC_BEGIN_ADDR_OF_MAIN_FUNCTION!"

#define IS_8B   0
#define IS_16B  1
#define IS_32B  2
#define VM_MEM_SIZE (64 * 1024)
class evm
{
private:
    uint16_t ip, sp, sfp;
    QByteArray bin_file_data;
    u_int8_t mem[VM_MEM_SIZE];
    int main_addr;
    u_int8_t *call_id;
    bool can_run;

public:
    evm();
    virtual ~evm(){}
    bool load_evm(QString &path);
    void run();
    void call_vm_by_id(u_int8_t id);
    bool evm_ready(){
        return can_run;
    }
    void set_call_id(u_int8_t id){
        *call_id = id;
    }
protected:
    u_int8_t *get_vm_mem_addr(){
        return mem;
    }
    void reset_evm();
    virtual int32_t call_user(uint8_t funcid, uint8_t argc, int32_t *argv) = 0;
    int32_t mem_read(uint16_t addr, u_int8_t bits);
    void mem_write(uint16_t addr, int32_t value, u_int8_t bits);
    void *OsMuMalloc(uint32_t size);
    void  OsMuFree(void *ptr);
    void set_evm_ready(bool can_or_not){
        can_run = can_or_not;
    }
    void set_call_id_addr(u_int8_t *addr){
        call_id = addr;
    }
private:
    int get_main_addr(){
        return bin_file_data.indexOf(BEFORE_MAIN_FLAG_STR) + sizeof(BEFORE_MAIN_FLAG_STR);
    }
    int16_t signext(uint16_t val, uint16_t mask);
    int32_t embedvm_pop();
    void embedvm_push(int32_t value);
    int32_t embedvm_local_read(int8_t sfa);
    void embedvm_local_write(int8_t sfa, int32_t value);
    void embedvm_exec();
    void embedvm_interrupt(uint16_t addr);
};

#endif // EVM_H
