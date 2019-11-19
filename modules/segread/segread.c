  #include <linux/init.h>
  #include <linux/kernel.h>
  #include <linux/module.h>
  #include <linux/sched.h>
  #include <linux/mm.h>
  #include <linux/syscalls.h>
  
  static int pid = 1;
  
  static void print_mem(struct task_struct *task){
  	struct mm_struct *mm;
  	struct vm_area_struct *vma;
  	int count = 0;
  	mm = task->mm;
  
  	printk("\nA struct mm_struct tem %d VMAs.\n", mm->map_count);
  	for (vma = mm->mmap ; vma ; vma = vma->vm_next) {
  		printk("%d)   Inicio em 0x%lx, Termina em 0x%lx\n", ++count, vma->vm_start, vma->vm_end);
  	}
  
  	printk("Segmento de Codigo start = 0x%lx, end = 0x%lx \n"
  			"Segmento de dados start = 0x%lx, end = 0x%lx\n"
  			"Segmento de pilha start = 0x%lx\n",
  			mm->start_code, mm->end_code,
  			mm->start_data, mm->end_data,
  			mm->start_stack);
  
  }
  
  
  static int mm_exp_load(void){
  	struct task_struct *task;
  	printk("Processo ID: %d\n", pid);
  	for_each_process(task) {
  		if ( task->pid == pid) {
  			printk("%s[%d]\n", task->comm, task->pid);
  			print_mem(task);
  		}
  	}
  	return 0;
  }
  
  static void mm_exp_unload(void){
  	printk("Saindo do módulo.\n");
  }
  
  module_init(mm_exp_load);
  module_exit(mm_exp_unload);
  module_param(pid, int, 0);
  
  MODULE_AUTHOR ("Carlos Moratelli");
  MODULE_DESCRIPTION ("Imprime os segmentos de memória de um processo.");
  MODULE_LICENSE("GPL");

