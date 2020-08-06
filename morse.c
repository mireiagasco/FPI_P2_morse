#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
typedef struct
{
    char caracter;
    char codificacio[6];

} lletra_t;

void carregar_diccionari(lletra_t dicc[], FILE *f_morse)
{
    int i, j;
    char dada[6];
    i=0;
    fscanf(f_morse,"%s",dada);
    while (!feof(f_morse))    /*mentre no s'acabi el fitxer*/
    {
        dicc[i].caracter=dada[0]; /*copiem la lletra*/
        fscanf(f_morse,"%s", dada);
        j=0;
        while (dada[j]!='*')
        {
            dicc[i].codificacio[j]=dada[j];   /*copiem la codificació sense l'asterisc*/
            j++;
        }
        dicc[i].codificacio[j]='\0';  /*col·locquem el sentinella*/
        fscanf(f_morse,"%s", dada);
        i++;
    }
}

void tornar_menu (int*ac)
{
    char s_n;

    printf("Vol realitzar una nova accio (tornar al menu)?   (escrigui 's' per a si i 'n' per a no)\n");
    scanf(" %c",&s_n);
    while (s_n!='S' && s_n!='s' && s_n!='N' && s_n!='n')
    {
        printf("Resposta no valida, escrigui 's' per a si i 'n' per a no\n");
        scanf(" %c",&s_n);
    }
    if (s_n=='n'|| s_n=='N')
    {
        *ac=TRUE;
    }
}

void codificar_paraula (int*a, lletra_t d[])
{
    char paraula[17];
    int l, m;

    /*inici*/
    paraula[16]='\0';
    printf("Indiqui la paraula que vol codificar\n"); /*demanem la paraula a codificar*/
    scanf("%s", paraula);
    if (paraula[16]!='\0')
    {
        printf("La paraula es massa llarga (el maxim son 15 lletres)\n"); /*si la paraula és massa llarga*/
    }
    else
    {
        l=0;
        while (paraula[l]!='\0')    /*mentre no s'acabi la paraula*/
        {
            m=0;
            while (d[m].caracter!=paraula[l] && (d[m].caracter+32)!=paraula[l])  /*mentre no coincideixi la lletra de la paraula amb la del diccionari*/
            {
                m++;

            }
            printf("%s", d[m].codificacio); /*escrivim la codificació*/
            printf("   ");   /*escrivim tres espais entre lletra i lletra*/
            l++;
        }
        printf("\n");
        tornar_menu(a);   /*preguntarem si es vol tornar al menú*/
    }
}

void codificar_fitxer (int*f, lletra_t dic[])
{
    FILE*f_original;
    FILE*f_codificat;
    char frase[100];
    int k, n;

    f_original=fopen("original.txt","r");   /*obrim el fitxer a codificar*/
    if (f_original==NULL)   /*comprovem que s'ha obert bé*/
    {
        printf("Hi ha algun error amb el fitxer a codificar\n");
        system("pause");
    }
    else
    {
        f_codificat=fopen("codificat.txt", "w");   /*obrim el fitxer on desarem la codificació*/
        fgets(frase,100,f_original); /*obtenim la primera frase*/
        while (!feof(f_original))
        {
            k=0;
            while (frase[k]!='\n')  /*mentre no s'acabi la frase*/
            {
                while (frase[k]!=' ' && frase[k]!='\n')   /*mentre no s'acabi la paraula*/
                {
                    n=0;
                    while (dic[n].caracter!=frase[k] && (dic[n].caracter+32)!=frase[k])  /*mentre no coincideixi la lletra de la paraula amb la del diccionari*/
                    {
                        n++;
                    }
                    fprintf(f_codificat, "%s", dic[n].codificacio);  /*escrivim la codificacio de la lletra*/
                    fprintf(f_codificat,"   ");  /*escrivim tres esparis entre lletra i lletra*/
                    k++;
                }
                if (frase[k]==' ')  /*si s'ha acabat la paraula*/
                {
                    fprintf(f_codificat,"  "); /*escrivim dos espais més entre paraula i paraula*/
                    k++;
                }
            }
            if (frase[k]=='\n') /*si s'ha acabat la frase*/
            {
                fprintf(f_codificat,"\n");   /*canviem de línia*/
            }
            fgets(frase,100,f_original); /*obtenim la següent frase*/
        }
        fclose(f_original);
        fclose(f_codificat);
        printf("Fitxer codificat\n");
        tornar_menu(f);   /*preguntarem si es vol tornar al menú*/
    }
}

int iguals (char codi_morse[], lletra_t diccionar[], int q2)
{
    int r,trobat;

    /*inici*/

    trobat=FALSE;
    r=0;
    while ((((codi_morse[r]!='\0') || diccionar[q2].codificacio[r]!='\0'))&&(!trobat))
    {
        if (codi_morse[r]!=diccionar[q2].codificacio[r])
        {
            trobat=TRUE;
        }
        else
        {
            r++;
        }
    }
    return(!trobat);
}

char descodificar_morse (char llet[], lletra_t dicciona[])
{
    int q;
    char carac;

    /*inici*/
    q=0;
    while ((!iguals(llet,dicciona,q))&&(q<=36))    /*mentre no trobem la lletra corresponent i no s'acabi el diccionari*/
    {
        q++;
    }
    if (q>36)   /*si surt perquè no ha trobat el caràcter, retornarem un caràcter impossible*/
    {
        carac='*';
    }
    else    /*si surt perquè ha trobat el caràcter, el retornem*/
    {
        carac=dicciona[q].caracter;
    }
    return (carac);
}

void descodificar_fitxer (int*finalitzar, lletra_t diccion[])
{
    FILE *fit_cod;
    FILE *fit_descod;
    char lletra[6], element;
    int i, cont_espais;

    fit_cod=fopen("codificat.txt","r"); /*obrim el fitxer que volem descodificar*/
    if (fit_cod==NULL)  /*comprovem que s'ha obert bé*/
    {
        printf("Hi ha un error amb el fitxer que es vol descodificar\n");
        system("pause");
    }
    else    /*si el fitxer s'ha obert bé*/
    {
        fit_descod=fopen("descodificat.txt","w");
        fscanf(fit_cod,"%c",&element);  /*obtenim el primer element*/
        i=0;
        cont_espais=0;
        while (!feof(fit_cod))  /*mentre no s'acabi el fitxer*/
        {
            if (element!=' ' && element!='\n')    /*mentre no trobem un espai*/
            {
                if (cont_espais>1)
                {
                    cont_espais=0;
                    i=0;
                }
                lletra[i]=element;
                i++;
            }
            if (element==' ')
            {
                cont_espais++;
                if (cont_espais==1)
                {
                    lletra[i]='\0';
                    fprintf(fit_descod,"%c",descodificar_morse(lletra, diccion));
                }
                else
                {
                    if (cont_espais==5)
                    {
                        fprintf(fit_descod," ");
                    }
                }
            }
            if (element=='\n')
            {
                fprintf(fit_descod, "\n");
            }
            fscanf(fit_cod,"%c",&element);
        }
        printf("Fitxer descodificat\n");
        tornar_menu(finalitzar);
    }
}

int main()
{
    int acabar;
    FILE *fit_morse;
    lletra_t diccionari[36];
    char opcio;

    fit_morse=fopen("morse.txt","r");   /*obrim el fitxer en mode lectura*/
    if (fit_morse==NULL)    /*comprovem que s'ha obert bé*/
    {
        printf("Hi ha un error amb el fitxer de morse\n");    /*si no s'ha obert bé*/
    }
    else    /*si s'ha obert bé*/
    {
        carregar_diccionari(diccionari,fit_morse);
        fclose(fit_morse);
        acabar=FALSE;   /*inicialitzem a fals per fer que entri al bucle el primer cop*/
        while (!acabar) /*mentre no acabem*/
        {
            system("cls");
            printf("Menu:\n");
            printf("    -Codificar paraula (1)\n");
            printf("    -Codificar fitxer (2)\n");
            printf("    -Descodificar fitxer (3)\n");
            printf("    -Finalitzar (0)\n");
            printf("Indiqui quina accio vol realitzar:\n");
            scanf(" %c",&opcio);
            system("cls");

            while (opcio!='0' && opcio!='1' && opcio!='2' && opcio!='3')    /*si el valor introduit no és vàlid*/
            {
                printf("Valor no valid, indiqui un numero entre 0 i 3\n");
                scanf(" %c",&opcio);
                system("cls");
            }

            switch (opcio)
            {
            case '0':
                acabar=TRUE;
                break;
            case '1':
                codificar_paraula (&acabar, diccionari);
                break;
            case '2':
                codificar_fitxer (&acabar, diccionari);
                break;
            case '3':
                descodificar_fitxer (&acabar, diccionari);
                break;
            }
        }
    }
    return 0;
}
