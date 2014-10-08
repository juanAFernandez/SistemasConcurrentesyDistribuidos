##Problema seguro de examen, distinto y más complejo##

Sean los procesos P1,P2 Y P3, cuyas secuencias de instrucciones son las que se muestran en el cuadro.
Resuelva los siguientes problemas de sinronización (son independientes unos de otros).

a)P2 podrá pasar a ejecutar e solo si P1 ha ejecutado a o P3 a ejecutado g.

//Solución para sólo una pasada, no tiene mucho sentido pensar en el bucle (lo podemos eliminar).

s=0 //Semáforo

process P1;						
begin
	while true do begin
		a
		sem_signal(s)
		b
		c
	end
end

process P2;						
begin
	while true do begin
		d
		sem_wait(s)
		e
		f
	end
end

process P3;						
begin
	while true do begin
		g
		sem_signal(s)
		h
		i
	end
end


b)P2 podrá pasar a ejecutar e sólo si P1 ha ejecutado a y P3 ha ejecutado g.

//Solución para sólo una pasada, no tiene mucho sentido pensar en el bucle (lo podemos eliminar).

s=0 //Semáforo s
r=0 //Semáforo r

process P1;						
begin
	while true do begin
		a
		sem_signal(s)
		b
		c
	end
end

process P2;						
begin
	while true do begin
		d
		//Deben de esperarse dos condiciones distintas y por cada suposición uso un semáforo, el orden es irrelevante.
		sem_wait(s)
		sem_wait(r)
		e
		f
	end
end

process P3;						
begin
	while true do begin
		g
		sem_signal(r)
		h
		i
	end
end

c)Sólo cuando P1 haya ejecutado b, podrá pasar P2 a ejecutar e y P3 a ejecutar h.

//Solución para sólo una pasada, no tiene mucho sentido pensar en el bucle (lo podemos eliminar).

s=0 //Semáforo s
r=0 //Semáforo r

process P1;						
begin
	while true do begin
		a
		b
		sem_signal(s)
		sem_signal(r)
		c
	end
end

process P2;						
begin
	while true do begin
		d
		sem_wait(s)
		e
		f
	end
end

process P3;						
begin
	while true do begin
		g
		sem_wait(r)
		h
		i
	end
end

d)Sincroniza los procesos de forma que las secuencias b en P1, f en P2 y h en P3, sean ejecutadas como mucho por dos procesos simultáneamente.

//Solución para sólo una pasada, no tiene mucho sentido pensar en el bucle (lo podemos eliminar).

s=2 //Semáforo s

process P1;						
begin
	while true do begin
		a
		sem_wait(s)
		b
		sem_signal(s)
		c
	end
end

process P2;						
begin
	while true do begin
		d
		e
		sem_wait(s)
		f
		sem_signal(s)
	end
end

process P3;						
begin
	while true do begin
		g
		sem_wait(s)
		h
		sem_signal(s)
		i
	end
end