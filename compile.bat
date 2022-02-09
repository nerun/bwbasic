del bwbasic.exe
del *.o
del *.obj

call stdcomp bwbasic.c
call stdcomp bwb_int.c
call stdcomp bwb_tbl.c
call stdcomp bwb_cmd.c
call stdcomp bwb_prn.c
call stdcomp bwb_exp.c
call stdcomp bwb_var.c
call stdcomp bwb_inp.c
call stdcomp bwb_fnc.c
call stdcomp bwb_cnd.c
call stdcomp bwb_ops.c
call stdcomp bwb_dio.c
call stdcomp bwb_str.c
call stdcomp bwb_elx.c
call stdcomp bwb_stc.c
call stdcomp bwx_tty.c
call stdcomp bwd_cmd.c
call stdcomp bwd_fun.c

rem gcc -mno-cygwin -s -o bwbasic.exe *.o
bcc32 -O -ebwbasic.exe *.obj
