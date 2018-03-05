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
            <li class="active">Funkcionální testování</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">Testování: Akceptační testy</h1>
        </div>
    </div>
    <div class="row no-margin">
        <div class="panel panel-primary">
            <div class="panel-heading">Na co jsou akceptační?</div>
            <div class="panel-body">
                Akceptační testy probíhají na straně zákazníka a je to poslední stupeň testování softwaru před jeho definitivním převzetím zákazníkem.
                Pokud jsou již nějaké chyby nebo nesrovnalosti mezi aplikaci a specifikaci při akceptačním testování objeveny,
                tak je nutné v co nejkraším čase tyto chyby opravit a předat zákazníkovi k dalším testům.
                Pro akceptační testování webových aplikací se používá nástroj selenium.
            </div>
        </div>
    </div><!-- Na co je akceptační testování? -->
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Příklad</div>
            <div class="panel-body">
                <ul>
                    <li>Otevření prohlížeče Firefox</li>
                    <pre class="custom-code"><code class="java">import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.firefox.FirefoxDriver;

WebDriver driver = new FirefoxDriver();</code></pre>
                    <li>Zobrazní určitě stránky</li>
                    <pre class="custom-code"><code class="java">driver.get(page);</code></pre>
                    <li>Výběr elemetu/elementů</li>
                    <pre class="custom-code"><code class="java">//Výběr několika elementů podle klauzule BY
driver.findElements(BY);

// Výběr jednoho elemtu podle klauzole BY
driver.findElement(BY);
                    </code></pre>
                    <li>Typy klauzole BY</li>
                    <pre class="custom-code"><code class="java">import org.openqa.selenium.By;

// Výběr několika elementů podle klauzule BY
By.id(...);

// Vyběr elementů stejně jako v CSS
By.cssSelector(...);

// Výběr elementů podle jména
By.name(...);

// Výběr elementů podle jména html značky (tagu)
By.tagName(...);

// Výběr elemetů stejné jako při výběru z XML souboru
By.xpath(...);

// Výběr jednoho elemtu podle klauzole BY
driver.findElement(BY);</code></pre>
                    <li>Práce s načtenou stránkou</li>
                    <pre class="custom-code"><code class="java">// vlozi do elementu s id ID_ELEMENTU text VALUE
driver.findElement(By.id("ID_ELEMENTU")).sendKeys("VALUE");

// klikne na vybraný element
driver.findElement(By.id("ID_ELEMENTU")).sendKeys("VALUE");</code></pre>
                </ul>
            </div>
            <div class="panel-footer">
                Zdrojové kódy příkladů ke stažení <a href="${pageContext.request.contextPath}/example/tests-acceptable.zip">ZDE</a>.
            </div>
        </div>
    </div>

<jsp:include page="components/footer.jsp" />
</body>
</html>