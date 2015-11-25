#include "evm.h"
#include "QFile"
#include "uart_gui.h"
int32_t evm::mem_read(uint16_t addr, u_int8_t bits)
{
    if (bits == IS_8B){
        return mem[addr];
    }

    else if (bits == IS_16B){
        return (mem[addr] << 8) | mem[addr+1];
    }
    return (mem[addr] << 24) | (mem[addr+1] << 16) | (mem[addr+2] << 8) | (mem[addr+3]);

}

void evm::mem_write(uint16_t addr, int32_t value, u_int8_t bits)
{

    if (bits == IS_8B){
        mem[addr] = value;
    }
    else if (bits == IS_16B){
        mem[addr]   = value >> 8;
        mem[addr+1] = value;
    }
    else{
        mem[addr]   = value >> 24;
        mem[addr+1] = value >> 16;
        mem[addr+2] = value >> 8;
        mem[addr+3] = value;
    }
}
void *evm::OsMuMalloc(uint32_t size){
    return malloc(size);
}

void  evm::OsMuFree(void *ptr){
    return free(ptr);
}

int16_t evm::signext(uint16_t val, uint16_t mask)
{
    val = val & mask;				/*---------------------------- --*/
    if ((val & ~(mask >> 1)) != 0)/*	mask:abcdef..  ~(mask >> 1) 1~(abcde)*/
        val |= ~mask;				/*val&mask    */
    return val;
}

void evm::embedvm_interrupt(uint16_t addr)
{
    embedvm_push(sfp | 1);
    embedvm_push(ip);
       sfp =    sp;
       ip = addr;
}

int32_t evm::embedvm_pop()
{
    int32_t value = mem_read(sp, IS_32B);
    sp += 4;
    //printf("pop:%d\n", value);
    return value;
}

void evm::embedvm_push(int32_t value)
{
    //printf("push:%d\n", value);
    sp -= 4;
    mem_write(sp, value, IS_32B);
}

int32_t evm::embedvm_local_read(int8_t sfa)
{
    uint16_t addr = sfp - 4*sfa + (sfa < 0 ? +4 : -4);
    return mem_read(addr, IS_32B);
}

void evm::embedvm_local_write(int8_t sfa, int32_t value)
{
    uint16_t addr = sfp - 4*sfa + (sfa < 0 ? +4 : -4);
    mem_write(addr, value, IS_32B);
}
void evm::embedvm_exec()
{
    uint8_t opcode = mem_read(ip, false);
    uint16_t addr = 0;
    int32_t a = 0, b = 0;
    int8_t sfa = 0;

//printf("<<<<<<op:	%02x\n", opcode);
    switch (opcode)
    {
    case 0x00 ... 0x3f://push local
        sfa = signext(opcode, 0x3f);/*>=0x20,sfa =0xc0 | opcode, <0x20 sfa = opcode&0x3f*/
        embedvm_push(embedvm_local_read(sfa));/*>=0x20,sfa=-32~-1, <0x20:sfa=0~31*/
       ip++;
        break;
    case 0x40 ... 0x7f://pop local
        sfa = signext(opcode, 0x3f);/*>=0x60,sfa = 0xc0 | opcode, <0x60 sfa = opcode&0x3f=opcode - 0x40*/
        embedvm_local_write(sfa, embedvm_pop());/*>=0x60 sfa = -32~-1, sfa = 0 ~ 31*/
        ip++;
        break;
    case 0x80+0 ... 0x80+11:
    case 0xa8+0 ... 0xa8+5:
        b = embedvm_pop();
    case 0x80+12 ... 0x80+14:/*single */
        a = embedvm_pop();
        //printf("a:%d b:%d\n", a, b);
        switch (opcode) /*math */
        {
            case 0x80 +  0: embedvm_push( a + b);  break;
            case 0x80 +  1: embedvm_push(a - b);  break;
            case 0x80 +  2: embedvm_push(a * b);  break;
            case 0x80 +  3: embedvm_push(a / b);  break;
            case 0x80 +  4: embedvm_push(a % b);  break;
            case 0x80 +  5: embedvm_push(a << b); break;/*ptr shixian is here*/
            case 0x80 +  6: embedvm_push(a >> b); break;
            case 0x80 +  7: embedvm_push(a & b);  break;
            case 0x80 +  8: embedvm_push(a | b);  break;
            case 0x80 +  9: embedvm_push(a ^ b);  break;
            case 0x80 + 10: embedvm_push(a && b); break;
            case 0x80 + 11: embedvm_push(a || b); break;
            case 0x80 + 12: embedvm_push(~a);     break;
            case 0x80 + 13: embedvm_push(-a);     break;
            case 0x80 + 14: embedvm_push(!a);     break;
            case 0xa8 +  0: embedvm_push(a <  b); break;
            case 0xa8 +  1: embedvm_push(a <= b); break;
            case 0xa8 +  2: embedvm_push(a == b); break;
            case 0xa8 +  3: embedvm_push(a != b); break;
            case 0xa8 +  4: embedvm_push(a >= b); break;
            case 0xa8 +  5: embedvm_push(a >  b); break;

        }
        ip++;
        break;
    case 0x90 ... 0x97:/*push  immediate*/
        a = signext(opcode, 0x07);/*>= 0X94 a high 29bits = 1,low 3bits keep, < 0x94, a = opcode - 0x90 */
        if ((a & 0x04) != 0)/*a 1..*/
            a |= ~0x07;/*a --high 29 bits =1*/
        embedvm_push(a);
        ip++;
        break;
    case 0x98:/*push unsigned 1 bytes */
        a = mem_read(ip+1, IS_8B) & 0x00ff;
        embedvm_push(a);
        ip += 2;
        break;
    case 0x99:/*push signed 1 bytes */
        a = mem_read(ip+1, IS_8B) & 0x00ff;
        embedvm_push(signext(a, 0x00ff));
        ip += 2;
        break;
    case 0x9a:/*push 4 bytes */
        a = mem_read(ip+1, IS_32B);
        embedvm_push(a);
        ip += 5;
        break;
    case 0x9b:
        a = embedvm_pop();
        if (0) {
    case 0x9c:
            a = 0;
        }
        sp = sfp;
        ip = embedvm_pop();
        sfp = embedvm_pop();
        if ((sfp & 1) != 0)
            sfp &= ~1;
        else
            embedvm_push(a);
        break;
    case 0x9d:
        embedvm_pop();
        ip++;
        break;
    case 0x9e:
        addr = embedvm_pop();
        if (mem_read(ip+1, IS_8B) == 0x9d) {
            embedvm_push(sfp | 1);/*save function sfp ip*/
            embedvm_push(ip + 2);
        } else {
            embedvm_push(sfp);
            embedvm_push(ip + 1);
        }
        sfp = sp;
        ip = addr;
        break;
    case 0x9f:
        ip = embedvm_pop();
        break;
    case 0xa0 ... 0xa0+7:
        if ((opcode & 1) == 0) {/*a0,a2,a4,a6*/
            addr = ip + signext(mem_read(ip+1, IS_8B), 0x00ff);
            ip += 2;
        } else {/*a1, a3, a5, a7*/
            addr = ip + mem_read(ip+1, IS_16B);/*adress saved as 2 biytes*/
            ip += 3;
        }
        switch (opcode)
        {
        case 0xa0:
        case 0xa1:
            ip = addr;
            break;
        case 0xa2:
        case 0xa3:
            if (mem_read(ip, IS_8B) == 0x9d) {
                embedvm_push(sfp | 1);
                embedvm_push(ip + 1);
            } else {
                embedvm_push(sfp);
                embedvm_push(ip);
            }
            sfp = sp;
            ip = addr;
            break;
        case 0xa4:
        case 0xa5:
            if (embedvm_pop())
                ip = addr;
            break;
        case 0xa6:
        case 0xa7:
            if (!embedvm_pop())
                ip = addr;
            break;
        }
        break;
    case 0xae:
        embedvm_push(sp);/*save stack pointer*/
        ip++;
        break;
    case 0xaf:
        embedvm_push(sfp);/*save */
        ip++;
        break;
    case 0xb0 ... 0xb0+15:/*call function*/
        {
            uint8_t argc = embedvm_pop();/*get argc*/
            //int32_t argv[argc];/*32bits argv*/
            int32_t *argv = (int32_t *)OsMuMalloc(sizeof(int32_t) * argc);
            for (sfa=0; sfa<argc; sfa++)
                argv[sfa] = embedvm_pop();
            a = call_user(opcode - 0xb0, argc, argv);
            embedvm_push(a);/*call function return val ,save to stack*/
            OsMuFree(argv);
        }
        ip++;
        break;
    case 0xc0 ... 0xef:/*load store*/
        if ((opcode & 0x07) == 5) {
            /* this is a "bury" instruction */
            uint8_t depth = (opcode >> 3) & 0x07;/*c*/
            //int16_t stack[depth+1];
            int16_t *stack = (int16_t *)OsMuMalloc(sizeof(uint16_t) * (depth+1));
            for (sfa = 0; sfa <= depth; sfa++)
                stack[sfa] = embedvm_pop();
            embedvm_push(stack[0]);
            for (sfa = depth; sfa > 0; sfa--)
                embedvm_push(stack[sfa]);
            embedvm_push(stack[0]);
            ip++;
            OsMuFree(stack);
            break;
        }
        if ((opcode & 0x07) == 6) {
            /* this is a "dig" instruction */
            uint8_t depth = (opcode >> 3) & 0x07;
            //int16_t stack[depth+2];
            int16_t *stack = (int16_t *)OsMuMalloc(sizeof(uint16_t) *(depth+2));
            for (sfa = 0; sfa < depth+2; sfa++)
                stack[sfa] = embedvm_pop();
            for (sfa = depth+1; sfa > 0; sfa--)
                embedvm_push(stack[sfa-1]);
            embedvm_push(stack[depth+1]);
            ip++;
            OsMuFree(stack);
            break;
        }
        if(opcode == 0xc7)
        {
            uint16_t len = 0;
            int i = 0;
            addr = mem_read(ip+1, IS_16B);
            len =  mem_read(ip+3, IS_16B);
            ip += 5;
            for(;i < len; i++){
                mem_write(addr, mem_read(ip, IS_8B) & 0x00ff, IS_8B);
                addr++;
                ip++;
            }

        }
        sfa = ((opcode >> 3) & 0x07) == 4 || ((opcode >> 3) & 0x07) == 5 ? 2 : 0;/*32bits sfa<<1 bytes*/
        switch (opcode & 0x07)/*8bits sfa<<0 bytes*/
        {
        case 0:
            addr = mem_read(ip+1, IS_8B) & 0x00ff;
               ip += 2;
            break;
        case 1:
            addr = mem_read(   ip+1, IS_16B);/*var 4bits*/
               ip += 3;
            break;
        case 2:
            addr = embedvm_pop();
               ip++;
            break;
        case 3:
            addr = (embedvm_pop() << sfa) + (mem_read(   ip+1, IS_8B    ) & 0x00ff);/*address as popped + 1 byte (unsigned) argument*/
               ip += 2;
            break;
        case 4:
            addr = (embedvm_pop() << sfa) + mem_read(   ip+1, IS_16B    );/*32bits */
               ip += 3;
            break;
        }
        //printf("addr:%x\n", addr);
        switch ((opcode >> 3) & 0x07)
        {
        case 0:/*c0 - c7,load 8u*/
            if(opcode != 0xc7){
                embedvm_push(mem_read(addr, IS_8B    ) & 0x00ff);
            }
            break;
        case 1:/*c8 - cf,store 8u*/
            mem_write(addr, embedvm_pop(), IS_8B    );
            break;
        case 2:/*d0 - d7,load 8s*/
            embedvm_push(signext(mem_read(addr, IS_8B    ), 0x00ff));
            break;
        case 3:/*d8 - df,store 8s*/
            mem_write(addr, embedvm_pop(), IS_8B    );
            break;
        case 4:
            embedvm_push(mem_read(addr, IS_32B    ));/*e0-e7, load 32bits*/
            break;
        case 5:
            mem_write(addr, embedvm_pop(), IS_32B    );/*e8-ef, store 32bits*/
            break;
        }
        break;
    case 0xf0 ... 0xf7:/*push 1-8 ge local=0 to stack*/
        for (sfa = 0; sfa <= (opcode & 0x07); sfa++){
            embedvm_push(0);
        }
           ip++;
        break;
    case 0xf8 ... 0xff:
        a = embedvm_pop();
           sp += 4 + 4*(opcode & 0x07);
        embedvm_push(a);
           ip++;
        break;
    }
}
void evm::reset_evm()
{
    ip = 0xffff;
    sp = VM_MEM_SIZE;
    sfp = VM_MEM_SIZE;
    set_evm_ready(false);
}

evm::evm()
{
    reset_evm();
}

bool evm::load_evm(QString &path)
{
    QFile bin(path);
    if (bin.size() >= sizeof(mem)){
        ERROR("文件太大了，目前最大只支持64k");
        return false;
    }
    if (bin.open(QIODevice::ReadOnly) != true){
        qDebug("bin file open failed;%s!", path.toLatin1().data());
        set_evm_ready(false);
        return false;
    }
    bin_file_data = bin.readAll();
    bin.close();
    memcpy(mem, bin_file_data.data(), bin_file_data.length());
    main_addr = get_main_addr();
    if (main_addr < sizeof(BEFORE_MAIN_FLAG_STR)){
        INFO("文件内容错误！");
        set_evm_ready(false);
        return false;
    }
    //运行一次获取关键参数
    run();
    INFO("虚拟机加载成功！");
    return true;
}

void evm::run()
{
    embedvm_interrupt(main_addr);
    while (1) {
        if (ip == 0xffff) {
            break;
        }
        embedvm_exec();
    }
}
void evm::call_vm_by_id(u_int8_t id)
{
    if (!evm_ready()){
        return;
    }
    set_call_id(id);
    run();
}
