# Tema3SD-LinuxFileSystem
[Tema3 Structuri de date (2018-2019, seria CB)] 

Tema presupune crearea unei structuri de tip trie pentru reprezentarea unui sistem de fisiere Linux care efectueaza anumite comenzi.<br>
Enunt: https://acs.curs.pub.ro/2018/pluginfile.php/54368/mod_assign/intro/Tema%203%20-%20Linux%20Filesystem.pdf





#### IMPLEMENTARE
Structura de trie are in componenta un vector de fii(children) care retine
caractere alfa-numerice si '/' si '.', campurile 'isFile' si 'isDirectory' prin 
care se marcheaza sfarsitul numelui unui fisier, respectiv al unui director.
Directoarele se termina cu caracterul '/'.

Pentru a retine care este directorul curent se foloseste sirul "current_dir" 
care se modifica la fiecare comanda "cd".
Pentru inceput, directorul curent este root("/") si acesta se adauga in trie. Pe 
masura ce se citesc comenzile, se tine cont de caile absolute si cele relative. 
Prin folosirea functiilor "directory_path", "file_path" se obtine de 
fiecare data calea absoluta pentru fisierele/directoarele cu care se lucreaza.

Comanda "pwd" reprezinta afisarea directorului curent.

Comanda "cd" reprezinta de fapt modificarea directorului curent, in functie de calea 
data ca argument, tinandu-se cont de existeata directorului in trie.

Pentru comanda "ls" se tine cont de aparitia flagului "-F", fiind diferente la 
afisare. Se cauta in trie directorul al carui continut trebuie afisat, apelandu-se
functia "display_ls" care prin recursivitate creeaza sirul care trebuie afisat,
oprindu-se atunci cand gaseste sfarsit de fisier sau de director.

Comanda "mkdir" consta in obtinerea caii absolute pentru fiecare director, introducerea
lui in trie cu ajutorul functie "add_dir" care face parcurgerea prin trie pana gaseste 
directorul corespunzator si il marcheaza prin campul "isDirectory".
Comanda "touch" functioneaza pe acelasi principiu, considerandu-se acum ca se lucreaza
cu fisiere.

Comanda "rmdir" cauta in trie directorul dupa calea lui absoluta, testeaza daca directorul
are vreun continut, in caz negativ stergandu-l prin apelarea functiei "deleteDir" care sterge
recursiv fiecare caracter de la sfarsit spre inceput, eliminand si eticheta de director.
Comanda "rm" efectueaza un lucru similar, dar putand sterge si fisiere si directoare, 
indiferent daca este gol sau nu. Se utilizeaza functiile "deleteFile" si "deleteDir".


Comanda "cp" presupune obtinerea listei de fisiere care vor fi sterse si a directorului care
este ultimul cuvant din comanda. Lista de fisiere este salvata intr-un sir care urmeaza sa 
fie din nou spart cu strtok. Se afla noua cale la care fisierul urmeaza sa fie copiat, iar prin 
functia "touch", fisierul se creeaza acolo. Acest lucru se efectueaza pentru fiecare fisier.

Comanda "mv" are un comportament similar, dar in cazul acesteia se sterge vechea cale a 
fisierului, prin functia "deleteFile", dupa care se creeaza un fisier la noua cale.

Pentru fiecare comanda se tine cont de cazurile de eroare care pot aparea.
