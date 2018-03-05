<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="EJB: Interceptors"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>EJB</li>
                    <li class="active"> Interceptors</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">EJB:  Interceptors
                        <a href="${pageContext.request.contextPath}/example/ejb-all.zip" class="pull-right btn btn-success">Zdrojové kódy</a>
                    </h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <p class="custom-text-block">Interceptors jsou třdy, které obsahují metody, které se mají provést před provedením metod samotného EJB a případně i ovlivnit provedení metod EJB. Metoda s anotací <code>@AroundInvoke</code> se provede před provedením metody EJB a je možné ovlivnit provedení nebo neprovedení dané metody. Metody anotované s <code>@PostActivate</code> a <code>@PrePassivate</code> provádí EJB kontejner při zavedení a ukončení beanu.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.ejb;

import javax.ejb.PostActivate;
import javax.ejb.PrePassivate;
import javax.interceptor.AroundInvoke;
import javax.interceptor.InvocationContext;

public class HistoryInterceptor {

    @AroundInvoke
    public Object aroundInvoke(InvocationContext ic) throws Exception {
        System.out.println(&quot;AroundInvoke: Invoking method: &quot; + ic.getMethod());
        return ic.proceed();
    }

    @PostActivate
    public void postActivate(InvocationContext ic) {
        System.out.println(&quot;PostActivate: Invoking method: &quot; + ic.getMethod());
    }

    @PrePassivate
    public void prePassivate(InvocationContext ic) {
        System.out.println(&quot;PrePassivate: Invoking method: &quot; + ic.getMethod());
    }
}</code></pre></div>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.ejb;

import java.util.List;
import java.util.Stack;
import javax.annotation.PreDestroy;
import javax.ejb.Stateful;
import javax.interceptor.Interceptors;

@Stateful
@Interceptors({HistoryInterceptor.class})
public class HistoryBean implements HistoryRemoteIface {

    private Stack&lt;String&gt; history;

    public HistoryBean() {
        history = new Stack&lt;&gt;();
    }

    @Override
    public void push(String url) {
        history.push(url);
    }

    @Override
    public String pop() {
        return history.pop();
    }

    @Override
    public List&lt;String&gt; getHistory() {
        return history;
    }

    @PreDestroy
    public void preDestroy() {
        System.out.println(&quot;PreDestroy invocation&quot;);
    }
}</code></pre></div>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.ejb;

import java.util.List;
import javax.ejb.Remote;

@Remote
public interface HistoryRemoteIface {
    public void push(String url);
    public String pop();
    public List<String> getHistory();
}</code></pre></div>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsp">&lt;%@page import=&quot;javax.naming.NamingException&quot;%&gt;
&lt;%@page import=&quot;javax.naming.InitialContext&quot;%&gt;
&lt;%@page import=&quot;javax.ejb.EJB&quot;%&gt;
&lt;%@page import=&quot;example.ejb.*&quot;%&gt;
&lt;%@page contentType=&quot;text/html&quot; pageEncoding=&quot;UTF-8&quot;%&gt;

&lt;%!
    private HistoryRemoteIface history = null;

    public void jspInit() {
        try {
            history = (HistoryRemoteIface) new InitialContext()
                .lookup(&quot;java:global/ExampleEJB-1.0/HistoryBean&quot;);
        } catch (NamingException e) {
        }
    }
%&gt;

&lt;%
    String url = request.getParameter(&quot;url&quot;);
    if (url != null &amp;&amp; !url.isEmpty()) {
        history.push(url);
    }
%&gt;

&lt;!DOCTYPE html&gt;
&lt;html&gt;
    &lt;head&gt;
        &lt;meta http-equiv=&quot;Content-Type&quot; content=&quot;text/html; charset=UTF-8&quot;&gt;
        &lt;title&gt;EJB History&lt;/title&gt;
    &lt;/head&gt;
    &lt;body&gt;
        &lt;h1&gt;History&lt;/h1&gt;

        &lt;form method=&quot;POST&quot;&gt;
            &lt;p&gt;
                &lt;input type=&quot;text&quot; name=&quot;url&quot;&gt;
                &lt;button type=&quot;submit&quot; name=&quot;save&quot;&gt;Save&lt;/button&gt;
            &lt;/p&gt;
        &lt;/form&gt;

        &lt;h2&gt;History&lt;/h2&gt;
        &lt;ul&gt;
            &lt;% for (String u : history.getHistory()) { %&gt;
                &lt;li&gt;&lt;%= u %&gt;&lt;/li&gt;
            &lt;% } %&gt;
        &lt;/ul&gt;
    &lt;/body&gt;
&lt;/html&gt;</code></pre></div>
                    </div>
                </div>
            </div>
        </div>

        <jsp:include page="../components/footer.jsp" />
    </body>
</html>