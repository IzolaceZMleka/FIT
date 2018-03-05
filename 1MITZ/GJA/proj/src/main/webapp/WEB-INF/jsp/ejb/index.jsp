<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="EJB: Základní informace"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>EJB</li>
                    <li class="active">Základní informace</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">EJB: Základní informace</h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Co je EJB?</div>
                    <div class="panel-body">
                        <p class="custom-text-block">EJB jsou serverové komponenty umožňující modulární tvorbu aplikací. EJB je součástí specifikace API JavaEE a vychází z klasických beanů. Slouží k oddělení aplikační logiky od prezentační a perzistentní vrstvy, také slouží k propojení s ostatními technologiemi, např. JMS.</p>
                        <p class="custom-text-block">EJB běží ve speciálním virtuálním prostoru aplikačního serveru zvaném EJB kontejner. Kontejner spravuje životní cyklus EJB a jejich identifikaci, řeší komunikaci s klienty, dependency injection (naplnění proměnných, např. JSM, SQL připojení), řízení stavu a další.</p>
                        <p class="custom-text-block">Některé servery, které obsahují EJB kontejner: WildFly, Glassfish, Apache Tomcat.</p>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Rozhraní EJB</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Rozhraní u EJB je stejné jako běžné rozhraní v Javě (<em>interface</em>), ale u EJB se rozlišují na vzálené a lokální rozhraní deklarované anotací <code>@Remote</code> a <code>@Local</code>. Lokální rozhraní definuje metody, které bude možné používat ze stejného JVM. Vzdálené rozhraní definuje rozhraní, které bude možné používat i z jiného JVM vzdáleně. K volání vzdálených metod se může použít například RMI.</p>
                        <p class="custom-text-block">EJB nemusí implementovat žádné rozhraní, pak budou všechny jeho metody dostupné, ale pouze ve stejném JVM.</p>
                        <h4 class="custom-h4">Příklad vzdáleného rozhraní</h4>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">@Remote
interface ExampleRemoteInterface {
    // ...
}</code></pre></div>
                        <h4 class="custom-h4">Příklad EJB</h4>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">@Stateless
class ExampleBean implements ExampleRemoteInterface {
    // ...
}</code></pre></div>
                        <p class="custom-text-block">Implementace beanu může být také anotována jako <code>@Local</code> nebo <code>@Remote</code> s parametrem třídy rozhraní, ale <strong>není to nutné pokud má stejnou anotaci i rozhraní</strong>.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">@Stateless
@Remote(ExampleRemoteInterface.class)
class ExampleBean {
    // ...
}</code></pre></div>
                    </div>
                </div>
            </div>
            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Získání instance EJB</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Beany se získávají buď přes DI nebo pomocí JNDI, které slouží k získávání instancí vzdálených beanů na jinýn JVM nebo i jinde na síti.</p>
                        <p class="custom-text-block">JNDI jméno má následující formát:</p>
                        <div class="custom-code-block"><pre>java:global[/&lt;app-name&gt;]/&lt;module-name&gt;/&lt;bean-name&gt;[!&lt;fully-qualified-interface-name&gt;]
java:app/&lt;module-name&gt;/&lt;bean-name&gt;[!&lt;fully-qualified-interface-name&gt;]
java:module/&lt;bean-name&gt;[!&lt;fully-qualified-interface-name&gt;]</pre></div>
                        <p class="custom-text-block">Například:</p>
                        <div class="custom-code-block"><pre>java:global/ExampleEJB-1.0/AccountBean!example.ejb.AccountRemoteIface
java:app/ExampleEJB-1.0/AccountBean!example.ejb.AccountRemoteIface
java:module/AccountBean!example.ejb.AccountRemoteIface
java:jboss/exported/ExampleEJB-1.0/AccountBean!example.ejb.AccountRemoteIface
java:global/ExampleEJB-1.0/AccountBean
java:app/ExampleEJB-1.0/AccountBean
java:module/AccountBean</pre></div>
                        <h4 class="custom-h4">Pomocí dependency injection</h4>
                    <p class="custom-text-block">JNDI název není nutné uvádět pokud jsou ve stejném <em>war</em> archivu. Pomocí DI jde beany předávat do jiných EJB, managed beanů, servletů, ...</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">@Named(value = "beansContainer")
@SessionScoped
public class BeansContainer implements Serializable {

    @EJB(lookup = "java:global/ExampleEJB-1.0/CalculatorBean")
    private CalculatorRemoteIface calculator;

    public CalculatorRemoteIface getCalculator() {
        return calculator;
    }

    // ...
}</code></pre></div>
                        <h4 class="custom-h4">Pomocí JNDI vyhledávání</h4>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">CalculatorRemoteIface calculator = null;
try {
    final InitialContext ctx = new InitialContext();
    calculator = (CalculatorRemoteIface) ctx.lookup("java:global/ExampleEJB-1.0/CalculatorBean");
} catch (NamingException e) {}
</code></pre></div>
        </div>

        <jsp:include page="../components/footer.jsp" />
    </body>
</html>