#include "Monitoramento.h"
#include "Placa_mock.h"
#include "ListaEncadeada.h"
#include "elemento.h"
#include "Dado.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <conio2.h>
#include <windows.h>
#include <string>
#include <time.h>
#include <vector>

#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27
#define KB_SPACE 32
#define INI_TELA_X 4
#define INI_TELA_Y 2

using namespace std;

time_t converteParaTimestamp(unsigned int day, unsigned int month, unsigned int year, unsigned int hour, unsigned int minute, unsigned int second)
{
    struct tm tmdate = {0};
    tmdate.tm_year = year - 1900;
    tmdate.tm_mon = month - 1;
    tmdate.tm_mday = day;
    tmdate.tm_hour = hour;
    tmdate.tm_min = minute;
    tmdate.tm_sec = second;
    time_t t = mktime( &tmdate );
    return t;
}

string valData(string a)
{
    bool error=false;
    string mes, dia;
    int x=INI_TELA_X, y=INI_TELA_Y;
    if(a!="0")
    {
        if(a.size()!=10)error=true;
        else
        {
            for(unsigned int j=0; j<a.size(); j++)  //Verifica se a data possui caracteres al�m de n�meros e retira pontua��o
            {
                if(ispunct(a[j]))a.erase(j, 1);
                if(!isalnum(a[j]))error=true;
            }
            if(a.size()!=8) error=true;
        }
        mes = a.substr(2, 2);
        dia = a.substr(0, 2);
        if(mes>"12") error=true; //Verifica se a data � v�lida
        else
        {
            if((mes=="01" || mes=="03" || mes=="05" || mes=="07" || mes=="09" || mes=="11") && dia>"31")  (dia>"31"?error=!error:error);
            if(mes=="04" || mes=="06" || mes=="08" || mes=="10" || mes=="12")                           (dia>"30"?error=!error:error);
            if(mes=="02" && dia>"29")                                                                   (dia>"29"?error=!error:error);
        }
        (error?a="-1":a);
        gotoxy(x+24, y+15);
        textbackground(BLACK);
        if(a=="-1") cout << "Data Invalida, favor reinsira";
    }
    return a;
}

time_t* subConsulta()
{
//Controle do submenu da escolha de datas
    string a="-1", prim="-1", segu="-1";
    int x=INI_TELA_X, y=INI_TELA_Y;
    while(prim=="-1")
    {
        gotoxy(x+24, y+12);
        textbackground(MAGENTA);
        cout << "Informe a data inicial no formato: dd/mm/aaaa       ";
        gotoxy(x+24, y+13);
        cout << "Caso nao seja informado data, o padrao sera         ";
        gotoxy(x+24, y+14);
        textbackground(BLACK);
        clreol();
        textbackground(MAGENTA);
        cout<<  "a partir do inicio do registro:                     ";
        gotoxy(x+56, y+14);
        cin >> a;
        prim = valData(a);
    }
    while(segu=="-1")
    {
        gotoxy(x+24, y+12);
        textbackground(MAGENTA);
        cout << "Informe a data final no formato: dd/mm/aaaa         ";
        gotoxy(x+24, y+13);
        cout << "Caso nao seja informado data, o padrao sera         ";
        gotoxy(x+24, y+14);
        textbackground(BLACK);
        clreol();
        textbackground(MAGENTA);
        cout<<  "ate o final de todo registro:                       ";
        gotoxy(x+54, y+14);
        cin >> a;
        segu = valData(a);
    }
    time_t *resposta = new time_t[2];
    prim[2] = prim[6] = segu[2] = segu[6] = '\0';
    const char* dataI = prim.c_str();
    const char* dataF = segu.c_str();
    resposta[0] = converteParaTimestamp(atoi(&dataI[0]), atoi(&dataI[3]), atoi(&dataI[7]), 0, 0, 0);
    resposta[1] = converteParaTimestamp(atoi(&dataF[0]), atoi(&dataF[3]), atoi(&dataF[7]), 23, 59, 59);
    return resposta;
}

int subSalva(int i)
{
    bool yes=true;
    int conf=0, x=INI_TELA_X, y=INI_TELA_Y;
    while(conf!=KB_SPACE)
    {
        gotoxy(x+24, y+13);
        (yes?textbackground(GREEN):textbackground(MAGENTA));
        cout << " Y ";
        cout << "/";
        (!yes?textbackground(GREEN):textbackground(MAGENTA));
        cout << " N  ";
        textbackground(BLACK);
        conf = getch();
        if(conf == KB_LEFT || conf == KB_RIGHT) yes=!yes;
        (yes?i:i=-1); // verifica se o usario cancelou a op��o SALVAR EM DISCO
    }
    return i;
}

int subLimpa(int i)
{
    bool yes=true;
    int conf=0, x=INI_TELA_X, y=INI_TELA_Y;
    while(conf!=KB_SPACE)
    {
        gotoxy(x+24, y+16);
        (yes?textbackground(GREEN):textbackground(MAGENTA));
        cout << " Y ";
        cout << "/";
        (!yes?textbackground(GREEN):textbackground(MAGENTA));
        cout << " N  ";
        textbackground(BLACK);
        conf = getch();
        if(conf == KB_LEFT || conf == KB_RIGHT) yes=!yes;
        (yes?i:i=-1); // verifica se o usario cancelou a op��o SALVAR EM DISCO
    }
    return i;
}

int desenha(int i=-1, double t=0)
{
    int estado[3];
    if(i<4) estado[i-1]=1;
    i=-1;
    while(i<0)  //Garante a op��o de cancelar SALVAR EM DISCO
    {
        //gotoxy(1,1);
        int x=INI_TELA_X, y=INI_TELA_Y;
        int sel=0;
        int pos[13]= {1,0,0,0,0,0,0,0,0,0,0,0,0};
        clrscr();
        _setcursortype(_NOCURSOR);
        i++;
        gotoxy(x,   y);
        (estado[0]==1?textbackground(GREEN):textbackground(RED));
        cout << "Lampada";
        gotoxy(x+8, y);
        (estado[1]==1?textbackground(GREEN):textbackground(RED));
        cout << "Resistor";
        gotoxy(x+17, y);
        (estado[2]==1?textbackground(GREEN):textbackground(RED));
        cout << "Ventoinha ";
        while(sel!=KB_SPACE)  //desenha e faz o controle do MENU
        {
            gotoxy(x, ++(++y));
            textbackground(RED);
            cout << "Controles: UP DOWN SPACE";
            gotoxy(x, ++y);
            textbackground(BLUE);
            cout << "------------------------";
            gotoxy(x, ++y);
            (pos[0]==1?textbackground(LIGHTBLUE):textbackground(BLUE));
            cout << "| (Des)Liga Temperatura|";
            gotoxy(x, ++y);
            (pos[1]==1?textbackground(LIGHTBLUE):textbackground(BLUE));
            cout << "| Ligue Lampada        |";
            gotoxy(x, ++y);
            (pos[2]==1?textbackground(LIGHTBLUE):textbackground(BLUE));
            cout << "| Ligue Resistor       |";
            gotoxy(x, ++y);
            (pos[3]==1?textbackground(LIGHTBLUE):textbackground(BLUE));
            cout << "| Ligue Ventoinha      |";
            gotoxy(x, ++y);
            (pos[4]==1?textbackground(LIGHTBLUE):textbackground(BLUE));
            cout << "| Desligue Resistor    |";
            gotoxy(x, ++y);
            (pos[5]==1?textbackground(LIGHTBLUE):textbackground(BLUE));
            cout << "| Desligue Ventoinha   |";
            gotoxy(x, ++y);
            (pos[6]==1?textbackground(LIGHTBLUE):textbackground(BLUE));
            cout << "| Desligue Lampada     |";
            gotoxy(x, ++y);
            textbackground(BLUE);
            cout << "------------------------";
            gotoxy(x, ++y);
            cout << "|";
            (pos[7]==1?textbackground(GREEN):textbackground(RED));
            cout << " Consultar Leituras   ";
            textbackground(BLUE);
            cout << "|";
            gotoxy(x, ++y);
            cout << "|";
            (pos[8]==1?textbackground(GREEN):textbackground(RED));
            cout << " Salvar as leituras   ";
            textbackground(BLUE);
            cout << "|";
            gotoxy(x, ++y);
            cout << "|";
            (pos[9]==1?textbackground(GREEN):textbackground(RED));
            cout << " Exportar GNU         ";
            textbackground(BLUE);
            cout << "|";
            gotoxy(x, ++y);
            cout << "|";
            (pos[10]==1?textbackground(GREEN):textbackground(RED));
            cout << " Expotar CSV          ";
            textbackground(BLUE);
            cout << "|";
            gotoxy(x, ++y);
            cout << "|";
            (pos[11]==1?textbackground(GREEN):textbackground(RED));
            cout << " Limpar Registros     ";
            textbackground(BLUE);
            cout << "|";
            gotoxy(x, ++y);
            cout << "|";
            (pos[12]==1?textbackground(GREEN):textbackground(RED));
            cout << " Sair                 ";
            textbackground(BLUE);
            cout << "|";
            gotoxy(x, ++y);
            cout << "|";
            (pos[13]==1?textbackground(GREEN):textbackground(CYAN));
            cout << " Seleciona porta COM  ";
            textbackground(BLUE);
            cout << "|";
            gotoxy(x, ++y);
            cout << "------------------------";
            textbackground(BLACK);
            /*gotoxy(x, ++y);

            clreol();
            cout << "Temperatura: " <<t <<"C";*/
            sel = getch();
            if(sel==KB_DOWN)
            {
                if(i==13)
                {
                    i=0;
                    pos[i]=1;
                    pos[13]=0;
                }
                else
                {
                    pos[i]=0;
                    pos[i+1]=1;
                    i++;
                }
            }
            if(sel==KB_UP  )
            {
                if(i==0)
                {
                    i=13;
                    pos[i]=1;
                    pos[0]=0;
                }
                else
                {
                    pos[i]=0;
                    pos[i-1]=1;
                    i--;
                }
            }
            x=INI_TELA_X;
            y=INI_TELA_Y;
        }

        if(i==7 || i==9 || i==10)
        {
          // subConsulta(); //Controle do submenu de fornecer DATA
        }

        if(i==8)
        {
            i = subSalva(i);//Controle submenu da escolha SALVAR EM DISCO
        }
        if(i==11)
        {
            i = subLimpa(i);
        }
        gotoxy(x, y+20);
        textbackground(BLACK);//Reposiciona o cursor no final do MENU
    }
    return i;
}

int main()
{
    int porta = 3, opcao = 0;
    cout << "Insira o numero da porta serial: " << endl;
    cin >> porta;

    Placa *kit = new Placa(porta);
    Monitoramento *monitor = new Monitoramento(kit, 1000);
    monitor->lerContinuamente(true);

    cout << "Conectando a COM" << porta << "..." << endl << "Em caso de a placa estar conectada em outra porta, sera possivel escolher outra porta." << endl;
    Sleep(1500);

    ListaEncadeada<Dado> lista;
    Dado ultimaLeitura;

    //Thread para atualizar a temperatura em tempo real
    thread atualizaTemperatura([](Monitoramento *m)
    {
        ListaEncadeada<Dado> lista;
        Dado ultimaLeitura;
        Monitoramento *monitor = m;
        int x, y;
        Sleep(2000);    //Esperando a tela principal ser desenhada

        while (true)
        {
            if (monitor->estaLendoContinuamente())  //Caso uma leitura esteja sendo feita
            {
                //while(!kbhit());
                x = wherex();   //Salva a posição anterior do cursor
                y = wherey();
                gotoxy(4, 24);  //Posiciona o cursor no final
                clreol();       //Limpa a linha
                lista = monitor->getLeituras(time(NULL) -1);   //Pega todas as leituras de um segundo atrás
                ultimaLeitura = lista.pos(lista.getTam() -1);    //Pega ultima leitura
                cout << "Temperatura " << ultimaLeitura.temperatura << "C"; //Imprime a ultima leitura
                gotoxy(x, y);   //retorna para a posição anterior
            }
            Sleep(1000); //Espera para atualizar a temperatura
        }
    }, monitor);

    time_t *datas;
    ListaEncadeada<Dado> listaFiltrada;
    Dado lido;
    while(opcao!=12)
    {
        datas = NULL;
        if (monitor->estaLendoContinuamente())
        {
            lista = monitor->getLeituras(time(NULL) -1);   //Pega todas as leituras de um segundo atrás
            ultimaLeitura = lista.pos(lista.getTam() -1);    //Pega ultima leitura
        }
        opcao = desenha(opcao, ultimaLeitura.temperatura);
        switch(opcao)
        {
        case 0:
            if (monitor->estaLendoContinuamente())
                monitor->lerContinuamente(false);
            else
                monitor->lerContinuamente(true);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            datas = subConsulta();
            listaFiltrada = monitor->getLeituras(datas[0], datas[1]);
            if (listaFiltrada.getTam()>0)
            {
                monitor->lerContinuamente(false);
                clrscr();
                for (int i = 0; i < listaFiltrada.getTam(); i++)
                {
                    lido = listaFiltrada.pos(i);
                    cout << lido.dataFormatada() << "\t" << lido.temperatura << "C\tResistor " << (lido.resistor? "Ligado": "Desligado") << "\tVentoinha " << (lido.ventoinha? "Ligada": "Desligada") << endl;
                }
                system("pause");
                monitor->lerContinuamente(true);
            }
            break;
        case 8:
            monitor->salvarEmDisco();
            break;
        case 9:
            cout << "Nao implementado.";
            break;
        case 10:
            datas = subConsulta();
            monitor->exportarCSV(datas[0], datas[1]);
            break;
        case 11:
            monitor->lerContinuamente(false);
            monitor->limparRegistros();
            Sleep(500);
            monitor->lerContinuamente(true);
            break;
        case 12:
            Sleep(1000);
            delete monitor;
            Sleep(500);
            break;
        }
        Sleep(500);
        if (datas)
            delete datas;
    }
    /*int v;
    //    switchbackground(LIGHTGRAY);
    while(v!=12)
    {

        v = desenha(v);
        if(v!=12) cout << "Opcao " << v+1 << " Selecionada";
        else cout << "Selecionou 'Sair'. Adeus, Humano." <<endl;
        switch(v)
        {
        case 0:
            cout << " - Leu a Temperatura";
            break;
        case 1:
            cout << " - Ligou a Lampada";
            break;
        case 2:
            cout << " - Ligou o Resistor";
            break;
        case 3:
            cout << " - Ligou a Ventoinha";
            break;
        case 4:
            cout << " - Desligou o Resistor";
            break;
        case 5:
            cout << " - Desligou a Ventoinha";
            break;
        case 6:
            cout << " - Desligou a Lampada";
            break;
        case 8:
            cout << " - Salvou a porra toda";
            break;
        }
        Sleep(500);
    }*/
    return 0;
}

/*Placa* kitPIC = new Placa(3);	//Comunicação serial na COM3. Samuel, você pode criar um novo menu para a escolha da porta serial. Tudo que é necessario fazer é criar um novo objeto placa e substituir no objeto de classe monitoramento
 Monitoramento* monitor = new Monitoramento(kitPIC, 1000);

 ListaEncadeada<Dado> lista = monitor->getLeituras();
 Dado lido;
 if (true)
 for (int i = 0; i < lista.getTam(); i++) {
 lido = lista.pos(i);
 cout << lido.temperatura << "C \tResistor: " << lido.resistor
 << " \tVentoinha: " << lido.ventoinha << "  \t"
 << lido.dataFormatada() << endl;
 }
 cout << "Leituras: " << lista.getTam() << endl;
 //monitor->plotarGrafico();
 cout << "Esse é o fim do programa." << endl;*/
