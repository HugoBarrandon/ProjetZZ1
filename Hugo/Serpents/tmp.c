int testvoisin(grille,i,j)
{
    int nb = 0;
    int a,b
    for(a=min(i-1,0); a<=min(i+1,MAX-1);a++)
    {
        for(b=min(j-1,0); b<=min(j+1,MAX-1);b++)
        {
            nb+=grille[a][b];
        }
    }
    return nb;
}// caca