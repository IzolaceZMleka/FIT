Zadání projektu PDB pro rok 2017/2018

Prostorové, multimediální a temporální databáze
Odevzdání plánu projektu
do 27. 10. 2017 23:59 (pátek)
Do WISu nahraje jeden ze členů týmu PDF dokument s plánem projektu. V dokumentu
bude popsána specifikace aplikace (co bude poskytovat uživateli, jak se bude ovládat,
atp.; se zaměřením na správu dat) a databáze (co se bude ukládat, tj. jaká prostorová,
multimediální a temporální data, co a jak se bude dotazovat; ne návrh/SQL, jen slovní
popis dat), a dále pak plán vývoje aplikace od data odevzdání plánu projektu do data
odevzdání finálního výsledku v nejvýše 14denních intervalech.

Odevzdání schéma databáze
do 10. 11. 2017 23:59 (pátek)
Do WISu nahraje jeden ze členů týmu validní SQL soubor/skript s DDL příkazy pro
tvorbu databázových objektů aplikace (schéma databáze, tj. schéma tabulek, omezení,
indexů, atp.). SQL skript musí být spustitelný bez chyb nad fakultním databázovým
serverem Oracle (chybné DROP neexistujících objektů jsou v pořádku). Schéma
databáze není závazné a může být později měněno, dle potřeb vývoje finální verze
aplikace.

Odevzdání finálního výsledku
do 11. 12. 2017 23:59 (pondělí)
Do WISu nahraje jeden ze členů týmu archív (ZIP, TGZ) s finálním obsahem repositáře
(ale bez adresáře .git), tj. zejména soubory se zdrojovými kódy, soubory nutné k
jednoduchému zprovoznění aplikace (Makefile, projekt v Maven, NetBeans, atp.),
programová dokumentace, popis zprovoznění aplikace a volitelně také uživatelská
dokumentace. Archiv může také obsahovat předkompilované strojově nezávislé kódy
(JAR, ne EXE!). V případě potřeby může být archív před nahráním do WISu rozdělen až
na 4 části, každá o maximální velikosti 2 MB.
V kořenovém adresáři archivu se musí nacházet soubor „body.txt“ s popisem rozdělení
práce na projektu (vizte níže).

Postup registrace týmu a založení repositáře
do 13. 10. 2017 23:59 (pátek)
Každý ze členů týmu si vytvoří (pokud již nemá) účet na gitlab.com s uživatelským
jménem dle vlastního uvážení a skutečným jménem a příjmením (můžete se rovnou
přihlásit VUT účtem pro Google Apps, tj. „Sign in with Google“, login@vutbr.cz a heslem
na VUT portál). Vedoucí týmu pak vytvoří repositář pro projekt s libovolným názvem
začínajícím textem „fit-pdb17-“ (bez uvozovek), typu GIT a s privátním přístupem
(Visibility level: Private). Poté přidá vedoucí v nastavení vytvořeného repositáře (sekce
Members) ostatní členy týmu (pomocí jejich uživatelských jmen) s právem zápisu (Project
Access: Developer/Master) a dále cvičícího (uživatelské jméno „rychly“) s právem čtení
(Project Access: Reporter).
Registraci provádí vedoucí týmu odesláním emailu na adresu <rychly@fit.vutbr.cz>. Email
bude mít předmět „PDB - registrace tymu“ a bude obsahovat loginy a jména všech členů
týmu, kde na prvním místě a vyznačený bude vedoucí týmu, a dále, na samostatném
řádku, odkaz na repositář (např. https://gitlab.com/rychly/fit-pdb17-restaurace).

Počet řešitelů
Řeší týmy mající 3 členy. Vybraný člen má úlohu vedoucího, přihlašuje se pouze on.

Specifikace zadání
Navrhněte a implementujte aplikaci typu klient-server, která demonstruje možnosti
uchování, zpracování, vyhledávání a prezentace prostorových, multimediálních a
temporálních dat v databázi Oracle.
Její použití v reálném prostředí je ponecháno na kreativitě autorů. Příkladem použití
mohou být aplikace pro cestovní nebo realitní kanceláře, systémy zahrnující skladovou
evidenci, evidenci zboží a obchodních transakcí, personální evidenci vč. evidence
oprávnění a zodpovědností, nebo GIS pro IZS, státní správu či energetické společnosti.
Prostorové databáze: Operace vkládání, mazání a změna uživatelských prostorových
dat pro alespoň 5 druhů geometrických entit, a to interaktivní formou (např. změna
souřadnic a velikosti entit tažením kurzoru na mapě). Dále netriviálních použití alespoň
jednoho prostorového operátoru, dalších 2 prostorových operátorů nebo funkcí a alespoň
3 analytických funkcí nebo operací demonstrujících práci nad prostorovými daty (pro
příklady vizte demonstrační cvičení).
Multimediální databáze: Operace vkládání, mazání a úprava multimédií (alespoň
statických obrázků, kde operace úprava může být např. jejich rotace) a jejich vyhledávání
dle multimediálního obsahu.
Temporální databáze: Operace vkládání, mazání a změna uživatelských temporálně
proměnlivých dat; vyhledávání nad daty platnými ve zvoleném období. V dokumentaci
popsaný návrh alespoň 5 netriviálních dotazů temporálního charakteru zapsaných
v TSQL2 nad v aplikaci zpracovávanými daty (SELECT temporálně proměnlivých dat nad
jedenou tabulkou [1×] a nad spojením více tabulek [2×], UPDATE [1×] a DELETE [1×]
temporálně proměnlivých dat) a popis jejich implementace v čistém jazyce SQL a použití
ve výsledné aplikaci (při vývoji zachovejte výše uvedené pořadí, tj. nejprve navrhněte
dotazy v TSQL2 a až pak je implementujte v aplikaci).
Klient: Implementační jazyk Java (po konzultaci s cvičícím možné i PL/SQL, C++, .NET,
atp.), aplikace s grafickým uživatelským rozhraním (použitelná nápověda, intuitivní
ovládání) na platformách Linux nebo Windows, která poskytuje možnosti prostorových,
temporálních a multimediálních databází včetně propojení s relačními daty.
• Uživatelské rozhraní musí umožňovat naráz prezentovat prostorové, multimediální
a temporální vlastnosti dat, např. temporální povaha dat nesmí být před
uživatelem skryta. Vhodně motivujte uživatele pro vyzkoušení aplikace, doplňte
komentářem, nebo jiným způsobem popište operace a jejich parametry.
• Je povoleno používat pouze vlastní výtvory a knihovny běžně dostupné se
základní distribucí implementačního jazyka (pro Javu tedy Java SE/EE knihovny,
dostupné např. s „JDK 7 with Java EE“), s distribucí použitého vývojového
prostředí (jDeveloper, NetBeans nebo Eclipse) a knihovny Oracle (ze cvičení).
Použití cizích zdrojových kódů (i částí) a dalších knihoven vyžaduje schválení
(žádost emailem cvičícímu s odkazem na knihovnu a zdůvodněním plánovaného
použití).
• Aplikace musí umožňovat volbu přihlašovacích údajů pro připojení
k databázovému serveru a (znovu-)naplnění tamní databáze tabulkami se sadou
ukázkových dat (implementováno např. formou s aplikací dodávaného
inicializačního skriptu, jehož příkazy aplikace na požádání přečte a odešle do
připojené databáze).
Server: fakultní databázový server Oracle Database 12c Enterprise Edition (informace k
připojení obdržíte), volitelně další jako (HTTP) aplikační server. Definice a vložení
uživatelských dat od tabulek po jednotlivé položky pouze prostřednictvím aplikace klienta.

Práce na projektu
V každém týmu se na projektu musí rovnoměrně podílet všichni jeho členové. Aby na
jednom projektu mohlo zároveň pracovat více lidí a nedocházelo k nekonzistentnostem, je
vyžadováno použití repositáře pro správu verzí (podrobnosti práce s repositářem
budou sděleny na přednášce a na stránkách předmětu).
Repositář projektu („repository“) musí vždy obsahovat veškerá data aktuálního stavu
řešení. Jednotlivé verze („revisions“) musí být odeslány odpovědným členem týmu (tj.
autorem změn) a musí obsahovat stručný popis změn (tj. neměl by být prováděn „commit“
bez komentáře; toto bude kontrolováno při hodnocení projektu).
Je doporučeno na projektu pracovat průběžně. Repositář projektu bude průběžně
kontrolován (vč. obsahu uložených zdrojových souborů), záznamy o změnách verzí a
průběžný obsah repositáře budou použity při závěrečném hodnocení projektu a
hodnocení činnosti jednotlivých členů týmu. Nerovnoměrná aktivita v průběhu řešení,
zejména začátek řešení v druhé půli nebo až ke konci semestru, povede ke ztrátě bodů
při závěrečném hodnocení.
Každý člen týmu by měl mít přehled o průběžné práci ostatních a to zejména pomocí
nástroje pro správu verzí (průkazné záznamy o změnách a skutečný obsah repositáře).
První vzájemnou kontrolu je nutné provést ještě před polovinou semestru (před polosemestrálním
testem), další kontroly jsou pak doporučeny v týdenních intervalech
(provádí všichni členové týmu přičemž společné zápisy z kontrol je vhodné ukládat do
repositáře; potíže vzniklé zanedbáním těchto kontrol nebudou při hodnocení projektu
omluveny). V případě selhání spolupráce, nemožnosti vzájemné domluvy a řešení na fóru
PDB je doporučeno obrátit se na cvičícího. Změny v týmu jsou možné nejpozději 21 dní
před odevzdáním projektu.

Dokumentace
Programová dokumentace vygenerovaná automaticky z řádně komentovaných
zdrojových kódů použitím příslušného nástroje (Javadoc, Doxygen). U každého modulu
uveďte jeho autora, název, popis funkcionality a způsob použití.
Uživatelskou dokumentaci vypracujte dle charakteru aplikace. V případě dostatečně
intuitivního uživatelského rozhraní postačí dokumentace například ve formě jednoduché
nápovědy v samotné aplikaci.
Nezapomeňte vypracovat a s projektem odevzdat také dokumentaci temporálních
dotazů vč. popisu jejich implementace (dle požadavků v sekci „Specifikace zadání“).

Hodnocení
Pro hodnocení je potřeba mít včas odevzdané všechny výsledky (plán projektu,
schéma databáze, finální výsledek). Tyto výsledky se hodnotí společně při závěrečné
obhajobě projektu, kdy si členové týmu mezi sebe rozdělí celkem 60 bodů snížené o 20
bodů za každého člena, který se řešení neúčastnil. Standardní počet bodů na člena týmu
je 0–20, maximální 25 bodů. Návrh na rozdělení bodů musí být obsažen v souboru
„body.txt“ odevzdávaném do WISu jako součást projektu (v kořenovém adresáři).
Při obhajobě projektu může být navržené rozdělení bodů upraveno konzultantem podle
skutečnosti (zejména na základě průkazných záznamů o změnách a průběžném obsahu
repositáře, vizte část „Práce na projektu“), vždy však individuálně pro jednotlivé členy (bez
nutného převodu odebraných nebo přidaných bodů ostatním). Členům týmu, jejichž práce
bude uměle nadhodnocena (jejich větší podíl na řešení projektu nebude u obhajoby
prokazatelný), budou nadbytečné body odebrány (bez náhrady pro ostatní členy). V
případě podhodnocení práce mohou být body přidány. Přerozdělení bodů mezi členy
týmu nebude po odevzdání umožněno.
V případě, že v průběhu řešení přestane jeden či více členů na projektu pracovat a budou
proto hodnoceni 0 body, stačí zbývajícím členům týmu pro úspěšné vyřešení projektu
vypracovat poměrnou část zadání (např. původně tříčlenný tým může úspěšně odevzdat
při řešení pouze dvěma členy projekt splňující 2/3 požadavků zadání, více je nad rámec
povinností a může být hodnoceno prémiovými body). Funkčnost (částečného) řešení však
musí být možné předvést u obhajoby (tj. funkce nedostupné z uživatelského rozhraní či v
aplikaci nepoužité nebudou při hodnocení započítány, přestože by byly implementovány).
Soubor „body.txt“ obsahuje 3 řádky formátu xlogin00 <procenta> určující procentuální
rozdělení bodů podle názorů řešitelů projektu. Každá hodnota musí být v rozsahu 50–
125 % nebo 0 a jejich součet nesmí přesáhnout 300 % snížené o 100 % za každého
člena, který se řešení neúčastnil. Soubor popisuje také mapování jednotlivých částí
aplikace na jejich autory a další informace o postupu řešení tak, aby bylo možné určit
rozsah a charakter práce vykonaný jednotlivými členy.
Vzorový obsah souboru „body.txt“:
xaaaaa01 125 %
xbbbbb02 75 %
xccccc03 0
Vedoucím projektu je Jára da Cimrman (xaaaaa01), pracoval na
komunikaci se serverem a uživatelském rozhraní... . Jan z Rokycan
(xccccc03) se řešení projektu neúčastnil vůbec.
Problémy v týmu řešte včas (ihned po odhalení při průběžné kontrole, nejpozději 21 dní
před odevzdáním) vzájemnou domluvou, případně na fóru PDB nebo osobní konzultací (s
jasným prokázáním nenadálých okolností či jinak nestandardního průběhu řešení).

Způsob hodnocení
Pro získání plného počtu bodů musí být odevzdány všechny výsledky a na obhajobě
prezentována funkční aplikace splňující všechny body zadání (vizte sekce „Specifikace
zadání“, zejména její části popisující požadavky prostorové, multimediální a temporální
databáze), včetně odpovídající dokumentace (vizte sekce „Dokumentace“). Navíc musí
být prokazatelná průběžná práce na projektu během semestru (na základě průkazných a
komentovaných záznamů o změnách v repositáři, vizte část „Práce na projektu“).
Za výrazně nadstandardní řešení obsahující další funkcionalitu či prvky neuvedené
explicitně v zadání lze navíc získat prémiové body v maximální výši 25 % ze základu
bodů (nicméně jednotlivý řešitel nemůže celkově získat více než 25 bodů). Zde je
ponechán prostor pro iniciativu řešitelského týmu (např. implementace obecného
překladače TSQL2 na SQL pro zadávání uživatelských temporálních dotazů; „živý“ náhled
na prostorová data reflektující právě prováděné změny dat ostatními klienty databáze;
mapování v aplikaci používaných perzistentních objektů s prostorovými, multimediálními a
temporálními vlastnostmi na odpovídající data post-relační databáze a zpět; atp.).
Nesplnění formálních požadavků může způsobit, že projekt nebude hodnocen.
Projekty neobhájené nebo odevzdané po termínu nebudou hodnoceny.
Prohřešek proti akademické morálce či etice bude navíc vyšetřován.

Konzultanti
cvičení a projekty: RNDr. Marek Rychlý, Ph.D. <rychly@fit.vutbr.cz>
přednášky: Doc. Dr. Ing. Dušan Kolář <kolar@fit.vutbr.cz>
