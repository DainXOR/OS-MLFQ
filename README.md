Analisis

¿Qué ocurre si el boost es muy frecuente?
Si el priority boost ocurre demasiado seguido, los procesos regresan constantemente a la cola de mayor prioridad. Esto reduce el efecto del MLFQ, ya que los procesos que consumen mucho CPU no permanecen en colas inferiores el tiempo suficiente, haciendo que el comportamiento se parezca más a un Round Robin simple.

¿Qué ocurre si no existe boost?
Sin priority boost, los procesos que fueron degradados a colas de baja prioridad pueden permanecer allí indefinidamente. Si siguen llegando procesos nuevos a colas superiores, los de baja prioridad podrían recibir muy poco tiempo de CPU.

¿Cómo afecta un quantum pequeño en la cola de mayor prioridad?
Un quantum pequeño permite responder rápidamente a procesos interactivos o cortos, mejorando el tiempo de respuesta. Sin embargo, también incrementa el número de cambios de contexto y puede degradar rápidamente a los procesos que requieren más tiempo de CPU.

¿Puede haber starvation?
Sí. Si no existe un mecanismo como el priority boost, los procesos en colas de baja prioridad pueden sufrir starvation, ya que los procesos de mayor prioridad siempre serán atendidos primero.
