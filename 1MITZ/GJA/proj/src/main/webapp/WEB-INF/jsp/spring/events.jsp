<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="Spring: Události"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li class="active">Spring / Události</li>
                </ol>
            </div><!--/.row-->

            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">Spring: Události</h1>
                </div>
            </div><!--/.row-->

            <div clas="row">
                <div class="col-lg-12">
                    <div class="panel panel-primary">
                        <div class="panel-heading">Co to jsou události ve Springu?</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Aplikační události jsou jedním ze základních stavebních kamenů Springu a vycházejí z obecného architektonického vzoru EDA. K dispozici tak máme abstraktní třídu <i>ApplicationEvent</i>, která umožňuje vytvářet vlastní události nebo již předpřipravené události v rámci Springu.
                            </p>
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Vlastní aplikační událost - <i>ApplicationEvent</i></div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Pro definici vlastní události je nutné využít třídu <i>ApplicationEvent</i>, ze které v tomto případě vytvoříme jejího potomka <i>CustomEvent</i>. Ten bude obsahovat metodu <i>getInformation</i>, která může sloužit k získání důležitých informací pro uložení do databáze (např. při přihlášení nového uživatele).
                            </p>                   
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">public class CustomEvent extends ApplicationEvent {

    private String information;
    
    public CustomEvent(Object source, String information) {
        super(source);
    }

    public String getInformation(){
        return this.information;
    }

}</code>
                                </pre>
                            </div>   
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Vlastní aplikační událost - Publisher</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Pro sestrojení publisheru u vlastní události máme v zásadě dvě možnosti. První z nich je implementace rozhraní <i>ApplicationEventPublisherAware</i> a definice libovolně pojmenované třídy, která poté zavolá <i>publishEvent</i> u objektu <i>ApplicationEventPublisher</i>. Druhá možnost je obdobná, ale jednodušší - můžeme pomocí anotace <i>@Autowire</i> získat rovnou instanci třídy <i>ApplicationEventPublisher</i> bez implementace rozhraní.
                            </p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">public class CustomEventPublisher {

    @Autowired
    private ApplicationEventPublisher publisher;
    
    public void publishEvent(String information) {
        CustomEvent event = new CustomEvent(this, information);
        this.publisher.publishEvent(event);
    }
    
}
</code>
                                </pre>
                            </div>

                            <p class="custom-tect-block">V tomto případě se zvolená metoda nazývá <i>publishEvent</i> a její parametr obsahuje informace, které poté budou předány do dané události.</p>


                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Vlastní aplikační událost - Listener</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Poslední část pro dokončení vlastní události, který budeme potřebovat, je Listener. Jeho úkol je při vyvolání události provést potřebnou akci. Typicky se může jednat o invokaci metod DAO objektů. Spring obsahuje pro implementaci Listeneru rozhraní <i>ApplicationListener</i>.
                            </p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">public class CustomEventListener implements ApplicationListener<CustomEvent> {

    @Override
    public void onApplicationEvent(CustomEvent e) {
        System.out.println("DAO - save all information:" + e.getInformation());
    }

}
</code>
                                </pre>
                            </div>

                            <p class="custom-text-block">Nakonec ještě zaregistrujeme třídy <i>CustomEventPublisher</i> a <i>CustomEventListener</i> do beans.xml</p>
                            <p class="custom-text-block">Praktickou ukázku projektu <a href="">lze stáhnout zde</a>. Živá verze tohoto projektu je <a href="">pak dostupná zde.</a></p>


                        </div>
                    </div>
                </div>
            </div>

        </div>	<!--/.main-->

        <jsp:include page="../components/footer.jsp" />
    </body>
</html>
