/*
TITULO: A Cesar o que é de Cesar
Turma: 4311
Alunos: Felipe Piacentini Chemin e Cainã Schaurich Borges
*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <string.h>
#include <locale>

using namespace std;
int main()
{
    char alfabeto[26]={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char frequencia[26]={'A', 'E', 'O', 'S', 'R', 'I', 'N', 'D', 'M', 'U', 'T', 'C', 'L', 'P', 'V', 'G', 'H', 'Q', 'B', 'F', 'Z', 'J', 'X', 'K', 'W', 'Y'};
    char desejados[]={'D', 'E', 'D', 'O', 'D', 'A', 'E', 'M', 'U', 'M', 'C', 'H', 'T', 'O', 'T', 'A', 'M', 'E', 'T', 'E'};
    char indesejados[]={'X', 'Z', 'Z', 'X', 'Z', 'Y', 'Y', 'Z', 'X', 'Y', 'Y', 'X', 'Y', 'W', 'Y', 'W', 'Y', 'G', 'Y', 'K', 'K', 'Z', 'Z', 'K', 'Y', 'R', 'Y', 'V', 'K', 'X', 'Z', 'Z', 'K', 'K', 'Y', 'Y', 'W', 'W'};

    int quantidade[26]={0}, maior[26]={0}, shift[26]={0};     //QUANTIDADE: POSIÇÃO=ORDEM ALFABETICA | CONTEUDO=QUANTAS VEZES FOI REPETIDA        MAIOR: POSIÇÃO=MAIOR-->MENOR | CONTEUDO=NUMERO DAS LETRAS        SHIFT: POSIÇÃO=CHAVE | CONTEUDO=NUMERO DE VOTOS
    string nome, encoded, backup;

//---------------------------------------------------------------------------------ABRINDO ARQUIVO
    ifstream inFile;
    setlocale(LC_ALL,"");

    cout << "Qual é o nome do arquivo que você deseja abrir?" << endl;
    cin >> nome; cout << endl;
    inFile.open(nome+".txt", ios::in);
    if (! inFile)
     { cerr << "Arquivo " << nome <<  "nao pode ser aberto" << endl;
      return -1;
     }
        getline(inFile, encoded);
        backup = encoded; //BACKUP PARA PODER MANIPULAR A STRING "encoded" SEM PERDER A MENSAGEM ORIGINAL

     inFile.close();
//---------------------------------------------------------------------------------ABRINDO ARQUIVO

    for(int x=0; x<encoded.length(); x++){              // LINHAS 41-48 CONTAM QUANTAS VEZES CADA LETRA APARECE NA MENSAGEM CRIPTOGRAFADA
        for(int y=0; y<26; y++){
            if(encoded[x]==alfabeto[y]){
                    quantidade[y]++;
                    break;
            }
        }
    }

    for(int x=0; x<26; x++){                            // LINHAS 50-63 ORDENAM AS LETRAS NO VETOR "MAIOR", ONDE AS POSIÇÕES IINDICAM O TAMANHO (DECRESCENTE) E O CONTEÚDO INDICA A LETRA
        int teste=0;                                    // EXEMPLO: MAIOR[0]=2 QUER DIZER QUE A LETRA 2 (C) TEM O MAIOR VALOR DE REPETIÇÕES
        for(int y=0; y<26; y++){
            if(quantidade[y]>teste){
                teste=quantidade[y];
                maior[x]=y;
            }
        }
        quantidade[maior[x]]=0;
        if(teste == 0){
            maior[x]=30;
            break;
        }
    }

    for(int x=0; x<26; x++){                            // LINHAS 65-71 USA AS FREQUENCIAS DAS LETRAS PARA SABER QUAL É A CHAVE MAIS PROVAVEL
        int teste=alfabeto[maior[x]]-frequencia[x];
        if(maior[x]==30)break;
        if(teste<0){
            shift[teste+26]+=1;
        }else shift[teste]+=1;
    }

    int t_d = sizeof(desejados) / sizeof(desejados[0]);
    int t_i = sizeof(indesejados) / sizeof(indesejados[0]);

    for(int z=1; z<26; z++){                                                //LINHAS 76-95 PROCURAM PARES DE PALAVRAS ESPECIFICAS, DESEJADAS OU NÃO DESEJADAS,
        for(int k=0; k<encoded.length(); k++){                              //PARA AUMENTAR AS CHANCES DE ACHAR A CHAVE REAL
                encoded[k]-=z;
                if(encoded[k]<65)encoded[k]+=26;
        }
        for(int x=0; x<encoded.length()-1; x++){
            for(int y=0; y<t_d; y++){
                if(encoded[x] == desejados[y] && encoded[x+1] == desejados[y+1]){
                    shift[z]+=1;
                }
            }
            for(int i=0; i<t_i; i++){
                if(encoded[x] == indesejados[i] && encoded[x+1] == indesejados[i+1]){
                    shift[z]-=3;
                }
            }
        }

        encoded = backup;
    }


//---------------------------------------------------------------------------------CRIANDO ARQUIVO
    fstream outFile;

    outFile.open(nome+"_dec.txt", ios::out);
    if (! outFile)
     { cout << "Arquivo " << nome << " não pode ser aberto" << endl;
       return -1;
   }

    cout << "Mensagem criptografada: " << encoded << endl << endl;
    outFile << "Mensagem criptografada: " << encoded << endl << endl;

    for(int x=0; x<2; x++){                         // LINHAS 110-132 USA AS CHAVES COM MAIS CHANCES DE ESTAREM CORRETAS
        int teste=0;
        int testep;
        for(int y=0; y<26; y++){
            if(shift[y]>teste){
                teste=shift[y];
                testep=y;
            }
        }
        shift[testep]=0;
        for(int z=0; z<encoded.length(); z++){
            encoded[z]-=testep;
            if (encoded[z]<65)encoded[z]+=26;
        }
        cout << "Tentativa " << x+1 << ":" << endl;
        cout << "Mensagem: " << encoded << endl;
        cout << "Chave: " << testep << endl << endl;

        outFile << "Tentativa " << x+1 << ":" << endl;
        outFile << "Mensagem: " << encoded << endl;
        outFile << "Chave: " << testep << endl << endl;
        encoded = backup;
    }

    outFile.close();
//---------------------------------------------------------------------------------CRIANDO ARQUIVO

    return 0;
}
