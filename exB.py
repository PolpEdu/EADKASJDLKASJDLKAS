from sys import stdin, stdout
import sys

def readln():
    return stdin.readline().rstrip()

def outln(n):
    sys.stdout.write(str(n))
    sys.stdout.write("\n")


def movimentos(n, knights): 
    pos_visitadas = set() 

    for i in range(n): 
        x, y, m = knights[i] # x e y são as coord do knight atual e m é o nº de movimentos do knight atual
      
        moves_atual = {(x, y)} 

        for j in range(m):
            #calcular as posições alcançáveis do knight atual 
            moves_atual = {(a+1, b+2) for a, b in moves_atual} \
                       | {(a+1, b-2) for a, b in moves_atual} \
                       | {(a-1, b+2) for a, b in moves_atual} \
                       | {(a-1, b-2) for a, b in moves_atual} \
                       | {(a+2, b+1) for a, b in moves_atual} \
                       | {(a+2, b-1) for a, b in moves_atual} \
                       | {(a-2, b+1) for a, b in moves_atual} \
                       | {(a-2, b-1) for a, b in moves_atual}
            
            pos_visitadas = pos_visitadas | moves_atual
        
    return str(len(pos_visitadas))

    
def main(): 
    knights = []
    flag = True
    while (flag):
        n = int(readln())
        for i in range(n):
            string = stdin.readline().rstrip() 
            x, y, m = map(int, string.split())
            knights.append((x, y, m))
        outln(movimentos(n, knights))


if __name__ == "__main__":
    main()




