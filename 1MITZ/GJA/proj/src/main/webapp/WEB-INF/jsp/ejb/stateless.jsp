<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="EJB: Stateless"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>EJB</li>
                    <li class="active">Stateless</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">EJB: Stateless</h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <p class="custom-text-block">Bezstavové EJB neuchovávají stav mezi jednotlivými požadavky klienta. Pro obsluhu klienta je straně serveru přiřazena instance beanu, která je vyhrazena pro zpracování jeho požadavku. Bezstavové beany jsou vláknově bežpečné, ale nelze v nich mezi požadavky ukládat nějaké hodnoty, protože není zaručeno, že zůstanou při dalším požadavku klienta zachovány.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.ejb;

import javax.ejb.Remote;

@Remote
public interface CalculatorRemoteIface {
    public int add(int lft, int rgh);
    public int sub(int lft, int rgh);
}</code></pre></div>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.ejb;

import javax.ejb.Stateless;

@Stateless
public class CalculatorBean implements CalculatorRemoteIface {

    @Override
    public int add(int lft, int rgh) {
        return lft + rgh;
    }

    @Override
    public int sub(int lft, int rgh) {
        return lft - rgh;
    }
}</code></pre></div>
                    </div>
                </div>
            </div>

                    <div class="row no-margin" id="sec-jsp">
                        <div class="panel panel-primary">
                    <div class="panel-heading">Příklad použití v JSP</div>
                    <div class="panel-body">
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsp">&lt;%@page import=&quot;javax.naming.NamingException&quot;%&gt;
&lt;%@page import=&quot;javax.naming.InitialContext&quot;%&gt;
&lt;%@page import=&quot;javax.ejb.EJB&quot;%&gt;
&lt;%@page import=&quot;example.ejb.*&quot;%&gt;
&lt;%@page contentType=&quot;text/html&quot; pageEncoding=&quot;UTF-8&quot;%&gt;

&lt;%!
    private CalculatorRemoteIface calculator = null;

    public void jspInit() {
        try {
            calculator = (CalculatorRemoteIface) new InitialContext()
                .lookup(&quot;java:global/ExampleEJB-1.0/CalculatorBean&quot;);
        } catch (NamingException e) {}
    }
%&gt;

&lt;!DOCTYPE html&gt;
&lt;html&gt;
    &lt;head&gt;
        &lt;meta http-equiv=&quot;Content-Type&quot; content=&quot;text/html; charset=UTF-8&quot;&gt;
        &lt;title&gt;EJB Calculator&lt;/title&gt;
    &lt;/head&gt;
    &lt;body&gt;
        &lt;h1&gt;Calculator&lt;/h1&gt;

        &lt;%
            String op = request.getParameter(&quot;op&quot;);
            if (op != null &amp;&amp; !op.isEmpty()) {
        %&gt;
        &lt;%
            int result = 0;
            try {
                int x = Integer.parseInt(request.getParameter(&quot;x&quot;));
                int y = Integer.parseInt(request.getParameter(&quot;y&quot;));
                switch (op) {
                    case &quot;+&quot;: result = calculator.add(x, y); break;
                    case &quot;-&quot;: result = calculator.sub(x, y); break;
                    default: break;
                }
                %&gt;
                &lt;p&gt;&lt;%= x %&gt; &lt;%= op %&gt; &lt;%= y %&gt; = &lt;%= result %&gt;&lt;/p&gt;
                &lt;%
            } catch (NumberFormatException e) {
                %&gt;
                &lt;p style=&quot;color:red&quot;&gt;Something went wrong! :(&lt;/p&gt;
                &lt;%
            } catch (NullPointerException e) {
                %&gt;
                &lt;p&gt;EJB were not injected...&lt;/p&gt;
                &lt;%
            }
        %&gt;
        &lt;hr&gt;
        &lt;% } %&gt;

        &lt;form method=&quot;POST&quot;&gt;
            &lt;p&gt;X = &lt;input type=&quot;number&quot; name=&quot;x&quot;&gt;&lt;/p&gt;
            &lt;p&gt;Y = &lt;input type=&quot;number&quot; name=&quot;y&quot;&gt;&lt;/p&gt;
            &lt;p&gt;
                &lt;button type=&quot;submit&quot; name=&quot;op&quot; value=&quot;+&quot;&gt;X &amp;plus; Y&lt;/button&gt;
                &lt;button type=&quot;submit&quot; name=&quot;op&quot; value=&quot;-&quot;&gt;X &amp;minus; Y&lt;/button&gt;
            &lt;/p&gt;
        &lt;/form&gt;
    &lt;/body&gt;
&lt;/html&gt;</code></pre></div>
                    </div>
                </div>
            </div>
        </div>

        <jsp:include page="../components/footer.jsp" />
    </body>
</html>