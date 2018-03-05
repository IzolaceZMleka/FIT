<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<jsp:include page="components/header.jsp">
    <jsp:param name="title" value="Hlavní strana"/>
</jsp:include>
<body>
<jsp:include page="components/top-panel.jsp" />
<jsp:include page="components/menu.jsp" />

<div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">
    <div class="row">
        <ol class="breadcrumb">
            <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
            <li>Testování</li>
            <li class="active">JUnit</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">Testování: JUnit</h1>
        </div>
    </div>
    <div class="row no-margin">
        <div class="panel panel-primary">
            <div class="panel-heading">Co je JUnit?</div>
            <div class="panel-body">
                JUnit je framework pro jednotkové testy psaný v programovacím jazyce Java.
                Na jeho vývoji se podíleli vývojáři – Kent Beck a Erich Gamma.
                JUnit je pravděpodobně nejúspěšnější z rodiny xUnit frameworků, kterou odstartoval Kent Beck se svým SUnit pro Smalltalk.
                Jednotkové testy jsou základním kamenem vývojových technik test-driven development a Extrémního programování
                a má se za to, že právě JUnit stojí za vznikem těchto programovacích technik. JUnit byl později portován
                na mnoho dalších jazyků, např. C# (NUnit), Python (PyUnit), Fortran (fUnit), Perl (Test::Class a Test::Unit) a C++ (CPPUnit).
                Tato rodina frameworků pro jednotkové testy byla pojmenována xUnit.
            </div>
        </div>
    </div><!-- Co je JUnit? -->
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Novinky od verze 4.0</div>
            <div class="panel-body">
                Od verze 4.0 jsou v testech podporovány Java anotace. Testovací třída nemusí dědit např od <i>TestCase</i>.
                Metody nemusí začínat slove <i>test</i>
                <h4>Příklad:</h4>
                <p>Část zdrojové třídy:</p>
                <pre class="custom-code"><code class="java">/**
 * Jednoduchá kalkulačka
 */
public class Calculator {
    /**
     * Výsledek výpočtu
     */
    private double result = 0.0;

    public Calculator() {}

    public Calculator(double result) {
        this.result = result;
    }

    /**
     * Přičte k předchozímu výsledku hodnotu sum
     *
     * @param sum
     * @return
     */
    public Calculator sum(double sum) {
        result += sum;
        return this;
    }

    /**
     * Předchozí výsledek vydělí hodnotou divide
     *
     * @param divide
     * @return
     */
    public Calculator divide(double divide) throws IllegalArgumentException {
        if (divide == 0.0)
            throw new IllegalArgumentException("Argument 'divide' is zero");
        result /= divide;
        return this;
    }
    ...
}</code></pre>
                <h5>Testy pro tuto třídu (JUnit 3.8.1)</h5>
                <pre class="custom-code"><code class="java">public class CalculatorTest extends TestCase {
    public Calculator calculator = null;

    /**
     * Provede se před každým testem
     */
    public void setUp() {
        calculator = new Calculator(1.0);
    }

    public void testSum() {
        calculator.sum(2.5);
        assertEquals(3.5, calculator.getResult());
    }

    public void testDivide() {
        try {
            calculator.divide(2.0);
            assertEquals(0.5, calculator.getResult());
        } catch (IllegalArgumentException e) {
            // nesmí nastat chyba při dělení (nedělíme nulou)
            fail(e.getMessage());
        }
    }

    public void testDivideByZero() {
        try {
           calculator.divide(0.0);
           fail("Nelze dělit nulou");
        } catch (IllegalArgumentException e) {
            assertEquals("Argument 'divide' is zero", e.getMessage());
        }
    }
    ...
}</code></pre>
                <h5>Testy pro tuto třídu (JUnit 4.12)</h5>
                <pre class="custom-code"><code class="java">public class CalculatorTest {

    public Calculator calculator;

    @Rule
    public ExpectedException thrown = ExpectedException.none();

    @Before
    public void setCalculator() {
        calculator = new Calculator(1.0);
    }

    @Test
    public void sum() {
        calculator.sum(2.5);
        assertEquals(new Double(3.5), new Double(calculator.getResult()));
    }

    @Test
    public void divide() {
        try {
            calculator.divide(2.0);
            assertEquals(new Double(0.5), new Double(calculator.getResult()));
        } catch (IllegalArgumentException e) {
            // nesmí nastat chyba při dělení (nedělíme nulou)
            fail(e.getMessage());
        }
    }

    /**
     * Nový způsob dostupný od verze 4
     */
    @Test
    public void divideByZeroNew() {
        thrown.expect(IllegalArgumentException.class);
        thrown.expectMessage("Argument 'divide' is zero");
        calculator.divide(0.0);
        fail("Nelze dělit nulou");
    }

    ...
}</code></pre>
            </div>
            <div class="panel-footer">
                Zdrojové kódy ke stažení <a href="${pageContext.request.contextPath}/example/junit-basic.zip">ZDE</a>.
            </div>
        </div>
    </div><!-- JUnit 3- vs 4+ -->
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Odstíňování závislostí</div>
            <div class="panel-body">
                Jak testovat třídu, která je závislá na jiných, které nechceme našimi testy ovlivnit?
                Pro to je dobré vytvořit si tzv. "falešné objekty", kterým se říká "mock objekty". Tyto objekty budou zastupovat při testování reálné objekty.
                <section>
                    <h4>Příklad: nejprve jak se pracuje s knihvnou <i>mockito</i></h4>
                    <p>Vy výchozím stavu je mock objekt prázdný a nemá definované ádné chování jinak řečeno, žádná metoda nic nedělá a pokud má návratovou hodnotu, tak vrací null</p>
                    <pre class="custom-code"><code class="java">import static org.mockito.Mockito.*;

public interface IClovek {

    /**
     * Udělá svůj pozdrav
     */
    public void pozdrav();

    /**
     * Vrátí jméno
     *
     * @return Jméno člověka
     */
    public String getJmeno();

    /**
     * Změní jméno
     */
    public void setJmeno(String jmeno);

}

// Vytvoření jednoduchého mock objektu
IClovek clovek = mock(IClovek.class);
                </code></pre>
                    <h5>Co se dá dělat s mock objektem?</h5>
                    <p>Například:</p>
                    <ol>
                    <li>Testování počtu volání jednotlivých metod:</li>
                    <pre class="custom-code"><code class="java hljs">// metoda pozdrav() musí být zavolána alepoň jednou
verify(clovek, atLeast(1)).pozdrav();
// metoda setJmeno() musí být zavolána pouze jednou
verify(clovek, times(1)).setJmeno("Karel");</code></pre>
// metoda getJmeno() nesmí být volána ani jednou
verify(clovek, never()).setJmeno("Karel");</code></pre>
                    <li>Nastavení návratové hodnoty:</li>
                    <pre class="custom-code"><code class="java hljs">// pokud se metoda getJmeno() zavolá, vrátí "Karel Novák"
when(clovek.setJmeno()).thenReturn("Karel Novák");</code></pre>
                    <li>Nastavení návratové hodnoty podruhé:</li>
                    <pre class="custom-code"><code class="java hljs">// pokud se metoda getJmeno() zavolá, vrátí "Karel Novák" a při druhém volání vrátí "Pavel Nový"
when(clovek.setJmeno()).thenReturn("Karel Novák").thenReturn("Pavel Nový");</code></pre>
                    <pre class="custom-code"><code class="java hljs"></code></pre>
                </ol>
                </section>
                <section>
                    <h4>Příklad: použtí mockito v praxi</h4>
                    <p>Třída Clovek potřebuje při vytváření instaci třídy implementující rozhraní ILogger</p>
                    <pre class="custom-code"><code class="java">public interface ILogger {
    /**
     * Zaznamená nějakou zprávu
     *
     * @param message Zpráva k zaznamenání
     */
    public void log(String message);

    /**
     * Ověří jestli zapnutý mod pro výpis zpráv na stdout
     *
     * @return
     */
    public boolean isEnabledStdOutMode();

    /**
     * Ověří jestli je zapnutý režim pro ukládání zpráv do souboru
     *
     * @return
     */
    public boolean isEnabledFileMode();
}

public class Clovek {
    ...

    public Clovek(ILogger logger, String jmeno, Integer vek, String pozdrav) {
        this.logger = logger;
        this.jmeno = jmeno;
        this.vek = vek;
        this.pozdrav = pozdrav;
    }

    /**
     * Napíše svuj pozdrav
     */
    public void pozdrav() {
        logger.log("byla zavolána metoda pozdrav");
        System.out.println(pozdrav);
    }

    /**
     * Zjisti zda je plnolety
     *
     * @return TRUE pokud je plnoletý
     */
    public boolean jsiPlnolety() {
        return vek >= 18;
    }
    ...
}</code></pre>
                    <p>Jelikož chceme otestovat třídu bez závislosti na třídu implementující dané rozhraní, tak to lze vyřešit jednoduchým mock objektem</p>
                    <pre class="custom-code"><code class="java">ILogger logger = mock(ILogger.class);
Clovek clovek = new Clovek(logger, "Karel Novák", 42, "Ahoj programátoři");</code></pre>
                    <p>Nyní můžeme testovat třídu Clovek a můžeme si libovně měnit chování mocku</p>
                    <pre class="custom-code"><code class="java">ILogger logger = mock(ILogger.class);
Clovek clovek = new Clovek(logger, "Karel Novák", 42, "Ahoj programátoři");</code></pre>
                </section>
            </div>
            <div class="panel-footer">
                Zdrojové kódy ke stažení <a href="${pageContext.request.contextPath}/example/junit-dependencies.zip">ZDE</a>.
            </div>
        </div>
    </div><!-- Odstíňovaní závislostí -->

    <jsp:include page="components/footer.jsp" />
</body>
</html>