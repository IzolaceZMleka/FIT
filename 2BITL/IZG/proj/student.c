/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id:$
 */

#include "student.h"
#include "transform.h"
#include "fragment.h"

#include <memory.h>
#include <math.h>


/*****************************************************************************
 * Globalni promenne a konstanty
 */

/* Typ/ID rendereru (nemenit) */
const int           STUDENT_RENDERER = 1;

const S_Material    MAT_WHITE_AMBIENT  = { 1.0, 1.0, 1.0, 1.0 };
const S_Material    MAT_WHITE_DIFFUSE  = { 1.0, 1.0, 1.0, 1.0 };
const S_Material    MAT_WHITE_SPECULAR = { 1.0, 1.0, 1.0, 1.0 };

/*****************************************************************************
 * Funkce vytvori vas renderer a nainicializuje jej
 */

S_Renderer * studrenCreate()
{
    S_StudentRenderer * renderer = (S_StudentRenderer *)malloc(sizeof(S_StudentRenderer));
    IZG_CHECK(renderer, "Cannot allocate enough memory");

    /* inicializace default rendereru */
    renderer->base.type = STUDENT_RENDERER;
    renInit(&renderer->base);

    /* nastaveni ukazatelu na upravene funkce */
    renderer->base.releaseFunc = studrenRelease;
	renderer->base.projectTriangleFunc = studrenProjectTriangle;
	
	
    /* inicializace nove pridanych casti */
	
	renderer->texture = loadBitmap(TEXTURE_FILENAME, &(renderer->text_width), &(renderer->text_height));

    return (S_Renderer *)renderer;
}

/*****************************************************************************
 * Funkce korektne zrusi renderer a uvolni pamet
 */

void studrenRelease(S_Renderer **ppRenderer)
{
    S_StudentRenderer * renderer;

    if( ppRenderer && *ppRenderer )
    {
        /* ukazatel na studentsky renderer */
        renderer = (S_StudentRenderer *)(*ppRenderer);

        /* pripadne uvolneni pameti */
		free(renderer->texture);
		renderer->texture=NULL;
		renderer = NULL;
        
        /* fce default rendereru */
        renRelease(ppRenderer);
    }
}

/******************************************************************************
 * Nova fce pro rasterizaci trojuhelniku s podporou texturovani
 * Upravte tak, aby se trojuhelnik kreslil s texturami
 * (doplnte i potrebne parametry funkce - texturovaci souradnice, ...)
 * v1, v2, v3 - ukazatele na vrcholy trojuhelniku ve 3D pred projekci
 * n1, n2, n3 - ukazatele na normaly ve vrcholech ve 3D pred projekci
 * x1, y1, ... - vrcholy trojuhelniku po projekci do roviny obrazovky
 */

void studrenDrawTriangle(S_Renderer *pRenderer,
                         S_Coords *v1, S_Coords *v2, S_Coords *v3,
                         S_Coords *n1, S_Coords *n2, S_Coords *n3,
						 S_Coords t1, S_Coords t2, S_Coords t3,
						 double h1, double h2, double h3,
                         int x1, int y1,
                         int x2, int y2,
                         int x3, int y3
                         )
{
    int         minx, miny, maxx, maxy;
    int         a1, a2, a3, b1, b2, b3, c1, c2, c3;
    int         s1, s2, s3;
    int         x, y, e1, e2, e3;
    double      alpha, beta, gamma, w1, w2, w3, z, u, v;
    S_RGBA      col1, col2, col3, color, tmp_color;

    IZG_ASSERT(pRenderer && v1 && v2 && v3 && n1 && n2 && n3);

    /* vypocet barev ve vrcholech */
    col1 = pRenderer->calcReflectanceFunc(pRenderer, v1, n1);
    col2 = pRenderer->calcReflectanceFunc(pRenderer, v2, n2);
    col3 = pRenderer->calcReflectanceFunc(pRenderer, v3, n3);

    /* obalka trojuhleniku */
    minx = MIN(x1, MIN(x2, x3));
    maxx = MAX(x1, MAX(x2, x3));
    miny = MIN(y1, MIN(y2, y3));
    maxy = MAX(y1, MAX(y2, y3));

    /* oriznuti podle rozmeru okna */
    miny = MAX(miny, 0);
    maxy = MIN(maxy, pRenderer->frame_h - 1);
    minx = MAX(minx, 0);
    maxx = MIN(maxx, pRenderer->frame_w - 1);

    /* Pineduv alg. rasterizace troj.
       hranova fce je obecna rovnice primky Ax + By + C = 0
       primku prochazejici body (x1, y1) a (x2, y2) urcime jako
       (y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

    /* normala primek - vektor kolmy k vektoru mezi dvema vrcholy, tedy (-dy, dx) */
    a1 = y1 - y2;
    a2 = y2 - y3;
    a3 = y3 - y1;
    b1 = x2 - x1;
    b2 = x3 - x2;
    b3 = x1 - x3;

    /* koeficient C */
    c1 = x1 * y2 - x2 * y1;
    c2 = x2 * y3 - x3 * y2;
    c3 = x3 * y1 - x1 * y3;

    /* vypocet hranove fce (vzdalenost od primky) pro protejsi body */
    s1 = a1 * x3 + b1 * y3 + c1;
    s2 = a2 * x1 + b2 * y1 + c2;
    s3 = a3 * x2 + b3 * y2 + c3;

    if ( !s1 || !s2 || !s3 )
    {
        return;
    }

    /* normalizace, aby vzdalenost od primky byla kladna uvnitr trojuhelniku */
    if( s1 < 0 )
    {
        a1 *= -1;
        b1 *= -1;
        c1 *= -1;
    }
    if( s2 < 0 )
    {
        a2 *= -1;
        b2 *= -1;
        c2 *= -1;
    }
    if( s3 < 0 )
    {
        a3 *= -1;
        b3 *= -1;
        c3 *= -1;
    }

    /* koeficienty pro barycentricke souradnice */
    alpha = 1.0 / ABS(s2);
    beta = 1.0 / ABS(s3);
    gamma = 1.0 / ABS(s1);

    /* vyplnovani... */
    for( y = miny; y <= maxy; ++y )
    {
        /* inicilizace hranove fce v bode (minx, y) */
        e1 = a1 * minx + b1 * y + c1;
        e2 = a2 * minx + b2 * y + c2;
        e3 = a3 * minx + b3 * y + c3;

        for( x = minx; x <= maxx; ++x )
        {
            if( e1 >= 0 && e2 >= 0 && e3 >= 0 )
            {
                /* interpolace pomoci barycentrickych souradnic
                   e1, e2, e3 je aktualni vzdalenost bodu (x, y) od primek */
                w1 = alpha * e2;
                w2 = beta * e3;
                w3 = gamma * e1;

                /* interpolace z-souradnice */
                z = w1 * v1->z + w2 * v2->z + w3 * v3->z;
				double bot = (w1*(1/h1)+w2*(1/h2)+w3*(1/h3));
				u = (w1 * (t1.x/h1) + w2 * (t2.x/h2) + w3 * (t3.x/h3))/bot;
				v = (w1 * (t1.y/h1) + w2 * (t2.y/h2) + w3 * (t3.y/h3))/bot;
               
                /* interpolace barvy */
                color.red = ROUND2BYTE(w1 * col1.red + w2 * col2.red + w3 * col3.red);
                color.green = ROUND2BYTE(w1 * col1.green + w2 * col2.green + w3 * col3.green);
                color.blue = ROUND2BYTE(w1 * col1.blue + w2 * col2.blue + w3 * col3.blue);
                color.alpha = 255;
				
				tmp_color = studrenTextureValue((S_StudentRenderer *) pRenderer, u, v);
				
 				color.red = (color.red * tmp_color.red)/255;
 				color.green = (color.green * tmp_color.green)/255;
 				color.blue = (color.blue * tmp_color.blue)/255;

                /* vykresleni bodu */
                if( z < DEPTH(pRenderer, x, y) )
                {
                    PIXEL(pRenderer, x, y) = color;
                    DEPTH(pRenderer, x, y) = z;
                }
            }

            /* hranova fce o pixel vedle */
            e1 += a1;
            e2 += a2;
            e3 += a3;
        }
    }
}

/******************************************************************************
 * Vykresli i-ty trojuhelnik n-teho klicoveho snimku modelu
 * pomoci nove fce studrenDrawTriangle()
 * Pred vykreslenim aplikuje na vrcholy a normaly trojuhelniku
 * aktualne nastavene transformacni matice!
 * Upravte tak, aby se model vykreslil interpolovane dle parametru n
 * (cela cast n udava klicovy snimek, desetinna cast n parametr interpolace
 * mezi snimkem n a n + 1)
 * i - index trojuhelniku
 * n - index klicoveho snimku (float pro pozdejsi interpolaci mezi snimky)
 */

void studrenProjectTriangle(S_Renderer *pRenderer, S_Model *pModel, int i, float n)
{
    S_Coords    aa, bb, cc;             /* souradnice vrcholu po transformaci */
    S_Coords    naa, nbb, ncc;          /* normaly ve vrcholech po transformaci */
    S_Coords    nn;                     /* normala trojuhelniku po transformaci */
    int         u1, v1, u2, v2, u3, v3; /* souradnice vrcholu po projekci do roviny obrazovky */
    S_Triangle  * triangle;
    int         vertexOffset, normalOffset; /* offset pro vrcholy a normalove vektory trojuhelniku */
    int         i0, i1, i2, in;             /* indexy vrcholu a normaly pro i-ty trojuhelnik n-teho snimku */
    
    
    S_Coords    aa_n1, bb_n1, cc_n1;             /* souradnice vrcholu po transformaci */
    S_Coords    naa_n1, nbb_n1, ncc_n1;          /* normaly ve vrcholech po transformaci */
    S_Coords    nn_n1;                     /* normala trojuhelniku po transformaci */
    int         vertexOffset_n1, normalOffset_n1; /* offset pro vrcholy a normalove vektory trojuhelniku */
    int         i0_n1, i1_n1, i2_n1, in_n1;             /* indexy vrcholu a normaly pro i-ty trojuhelnik n-teho snimku */
    

    IZG_ASSERT(pRenderer && pModel && i >= 0 && i < trivecSize(pModel->triangles) && n >= 0 );
	
	double decimal = (n - ((int)n));
	
    /* z modelu si vytahneme i-ty trojuhelnik */
    triangle = trivecGetPtr(pModel->triangles, i);

    /* ziskame offset pro vrcholy n-teho snimku */
    vertexOffset = (((int) n) % pModel->frames) * pModel->verticesPerFrame;
	
	vertexOffset_n1 = (((int) n+1) % pModel->frames) * pModel->verticesPerFrame;

    /* ziskame offset pro normaly trojuhelniku n-teho snimku */
    normalOffset = (((int) n) % pModel->frames) * pModel->triangles->size;
	
	normalOffset_n1 = (((int) n+1) % pModel->frames) * pModel->triangles->size;

    /* indexy vrcholu pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
	
    i0 = triangle->v[ 0 ] + vertexOffset;
    i1 = triangle->v[ 1 ] + vertexOffset;
    i2 = triangle->v[ 2 ] + vertexOffset;
	
	i0_n1 = triangle->v[ 0 ] + vertexOffset_n1;
    i1_n1 = triangle->v[ 1 ] + vertexOffset_n1;
    i2_n1 = triangle->v[ 2 ] + vertexOffset_n1;

	
    /* index normaloveho vektoru pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
    in = triangle->n + normalOffset;
	
	in_n1 = triangle->n + normalOffset_n1;

    /* transformace vrcholu matici model */
    trTransformVertex(&aa, cvecGetPtr(pModel->vertices, i0));
    trTransformVertex(&bb, cvecGetPtr(pModel->vertices, i1));
    trTransformVertex(&cc, cvecGetPtr(pModel->vertices, i2));
	
	trTransformVertex(&aa_n1, cvecGetPtr(pModel->vertices, i0_n1));
    trTransformVertex(&bb_n1, cvecGetPtr(pModel->vertices, i1_n1));
    trTransformVertex(&cc_n1, cvecGetPtr(pModel->vertices, i2_n1));
	
	aa.x = aa.x + (aa_n1.x - aa.x) * decimal;
	aa.y = aa.y + (aa_n1.y - aa.y) * decimal;
	aa.z = aa.z + (aa_n1.z - aa.z) * decimal;
	
	bb.x = bb.x + (bb_n1.x - bb.x) * decimal;
	bb.y = bb.y + (bb_n1.y - bb.y) * decimal;
	bb.z = bb.z + (bb_n1.z - bb.z) * decimal;
	
	cc.x = cc.x + (cc_n1.x - cc.x) * decimal;
	cc.y = cc.y + (cc_n1.y - cc.y) * decimal;
	cc.z = cc.z + (cc_n1.z - cc.z) * decimal;

    /* promitneme vrcholy trojuhelniku na obrazovku */
    double h1 = trProjectVertex(&u1, &v1, &aa);
    double h2 = trProjectVertex(&u2, &v2, &bb);
    double h3 = trProjectVertex(&u3, &v3, &cc);

    /* pro osvetlovaci model transformujeme take normaly ve vrcholech */
    trTransformVector(&naa, cvecGetPtr(pModel->normals, i0));
    trTransformVector(&nbb, cvecGetPtr(pModel->normals, i1));
    trTransformVector(&ncc, cvecGetPtr(pModel->normals, i2));
	
	trTransformVector(&naa_n1, cvecGetPtr(pModel->normals, i0_n1));
    trTransformVector(&nbb_n1, cvecGetPtr(pModel->normals, i1_n1));
    trTransformVector(&ncc_n1, cvecGetPtr(pModel->normals, i2_n1));
	
	naa.x = naa.x + (naa_n1.x - naa.x) * decimal;
	naa.y = naa.y + (naa_n1.y - naa.y) * decimal;
	naa.z = naa.z + (naa_n1.z - naa.z) * decimal;
	
	nbb.x = nbb.x + (nbb_n1.x - nbb.x) * decimal;
	nbb.y = nbb.y + (nbb_n1.y - nbb.y) * decimal;
	nbb.z = nbb.z + (nbb_n1.z - nbb.z) * decimal;
	
	ncc.x = ncc.x + (ncc_n1.x - ncc.x) * decimal;
	ncc.y = ncc.y + (ncc_n1.y - ncc.y) * decimal;
	ncc.z = ncc.z + (ncc_n1.z - ncc.z) * decimal;

    /* normalizace normal */
	
	coordsNormalize(&naa);
    coordsNormalize(&nbb);
    coordsNormalize(&ncc);

    /* transformace normaly trojuhelniku matici model */
    trTransformVector(&nn, cvecGetPtr(pModel->trinormals, in));
	
	trTransformVector(&nn_n1, cvecGetPtr(pModel->trinormals, in_n1));
	
	nn.x = nn.x + (nn_n1.x - nn.x) * decimal;
	nn.y = nn.y + (nn_n1.y - nn.y) * decimal;
	nn.z = nn.z + (nn_n1.z - nn.z) * decimal;
	
    
    /* normalizace normaly */
    coordsNormalize(&nn);
	
	

    /* je troj. privraceny ke kamere, tudiz viditelny? */
    if( !renCalcVisibility(pRenderer, &aa, &nn) )
    {
        /* odvracene troj. vubec nekreslime */
        return;
    }

    /* rasterizace trojuhelniku */
    studrenDrawTriangle(pRenderer,
                    &aa, &bb, &cc,
                    &naa, &nbb, &ncc,
					triangle->t[0],triangle->t[1],triangle->t[2],
					h1,h2,h3,
                    u1, v1, u2, v2, u3, v3
                    );
}

/******************************************************************************
* Vraci hodnotu v aktualne nastavene texture na zadanych
* texturovacich souradnicich u, v
* Pro urceni hodnoty pouziva bilinearni interpolaci
* Pro otestovani vraci ve vychozim stavu barevnou sachovnici dle uv souradnic
* u, v - texturovaci souradnice v intervalu 0..1, ktery odpovida sirce/vysce textury
*/	

S_RGBA studrenTextureValue( S_StudentRenderer * pRenderer, double u, double v )
{
    /* ??? */
	
	u *= pRenderer->text_height;
	v *= pRenderer->text_width;
	int x = floor(u);
	int y = floor(v);
	double u_ratio = u - x;
	double v_ratio = v - y;
	double u_opposite = 1 - u_ratio;
	double v_opposite = 1 - v_ratio;
	S_RGBA hodnota;
	hodnota.blue = (pRenderer->texture[x*pRenderer->text_height+y].blue * u_opposite + pRenderer->texture[(x+1)*pRenderer->text_height+y].blue * u_ratio)
						* v_opposite + (pRenderer->texture[x*pRenderer->text_height+(y+1)].blue * u_opposite + pRenderer->texture[(x+1)*pRenderer->text_height+(y+1)].blue * u_ratio) * v_ratio;
	hodnota.red = (pRenderer->texture[x*pRenderer->text_height+y].red * u_opposite + pRenderer->texture[(x+1)*pRenderer->text_height+y].red * u_ratio)
						* v_opposite + (pRenderer->texture[x*pRenderer->text_height+(y+1)].red * u_opposite + pRenderer->texture[(x+1)*pRenderer->text_height+(y+1)].red * u_ratio) * v_ratio;
	hodnota.green = (pRenderer->texture[x*pRenderer->text_height+y].green * u_opposite + pRenderer->texture[(x+1)*pRenderer->text_height+y].green * u_ratio)
						* v_opposite + (pRenderer->texture[x*pRenderer->text_height+(y+1)].green * u_opposite + pRenderer->texture[(x+1)*pRenderer->text_height+(y+1)].green * u_ratio) * v_ratio;
//     unsigned char c = ROUND2BYTE( ( ( fmod( u * 10.0, 1.0 ) > 0.5 ) ^ ( fmod( v * 10.0, 1.0 ) < 0.5 ) ) * 255 );
    return hodnota;
// 	return makeColor( c, 255 - c, 0 );

}

/******************************************************************************
 ******************************************************************************
 * Funkce pro vyrenderovani sceny, tj. vykresleni modelu
 * Upravte tak, aby se model vykreslil animovane
 * (volani renderModel s aktualizovanym parametrem n)
 */

void renderStudentScene(S_Renderer *pRenderer, S_Model *pModel)
{
    /* test existence frame bufferu a modelu */
    IZG_ASSERT(pModel && pRenderer);

    /* nastavit projekcni matici */
    trProjectionPerspective(pRenderer->camera_dist, pRenderer->frame_w, pRenderer->frame_h);

    /* vycistit model matici */
    trLoadIdentity();

    /* nejprve nastavime posuv cele sceny od/ke kamere */
    trTranslate(0.0, 0.0, pRenderer->scene_move_z);

    /* nejprve nastavime posuv cele sceny v rovine XY */
    trTranslate(pRenderer->scene_move_x, pRenderer->scene_move_y, 0.0);

    /* natoceni cele sceny - jen ve dvou smerech - mys je jen 2D... :( */
    trRotateX(pRenderer->scene_rot_x);
    trRotateY(pRenderer->scene_rot_y);

    /* nastavime material */
    renMatAmbient(pRenderer, &MAT_WHITE_AMBIENT);
    renMatDiffuse(pRenderer, &MAT_WHITE_DIFFUSE);
    renMatSpecular(pRenderer, &MAT_WHITE_SPECULAR);

    /* a vykreslime nas model (ve vychozim stavu kreslime pouze snimek 0) */
    renderModel(pRenderer, pModel, onTimer(0));
}

/* Callback funkce volana pri tiknuti casovace
 * ticks - pocet milisekund od inicializace */
double onTimer( int ticks )
{
    /* uprava parametru pouzivaneho pro vyber klicoveho snimku
     * a pro interpolaci mezi snimky */
	static double frame = 0;
	if(ticks != 0)
	{
		frame = ticks/100.0;
	}
	return frame;
    /* ??? */
}

/*****************************************************************************
 *****************************************************************************/
