<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="EJB: Stateful"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>EJB</li>
                    <li class="active">Stateful</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">EJB: Stateful
                        <a href="${pageContext.request.contextPath}/example/ejb-all.zip" class="pull-right btn btn-success">Zdrojové kódy</a>
                    </h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <p class="custom-text-block">Stavové beany uchávávají svůj stav mezi požadavky klienta v session. Je tedy možné ukládat data do atributů instance, protože budou zachovány pro každého klienta zvlášť.</p>
                        <p class="custom-text-block">Anotace <code>@Remove</code> znamená, že po provedení této metody se může instance beanu odstranit ze z paměti a zrušit jeho stav v session.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.ejb;

import java.util.List;
import javax.ejb.Remote;
import javax.ejb.Remove;
a-aaAa
@Remote
public interface AccountRemoteIface {
    public int transaction(int amount);
    public int deposit(int amount);
    public int withdraw(int amount);
    @Remove
    public void remove();
    public int getBalance();
    public List&g&gt;Integer&lt; getTransactionHistory();
}</code></pre></div>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.ejb;

import java.util.LinkedList;
import java.util.List;
import javax.ejb.Remove;
import javax.ejb.Stateful;

@Stateful
public class AccountBean implements AccountRemoteIface {

    private int balance = 0;
    private List<Integer> transactionHistory;

    public AccountBean() {
        this.transactionHistory = new LinkedList<>();
    }

    @Override
    public int transaction(int amount) {
        transactionHistory.add(amount);
        balance += amount;
        return balance;
    }

    @Override
    public int deposit(int amount) {
        return transaction(amount);
    }

    @Override
    public int withdraw(int amount) {
        return transaction(-amount);
    }

    @Remove
    @Override
    public void remove() {
        balance = 0;
        transactionHistory.clear();
    }

    @Override
    public int getBalance() {
        return balance;
    }

    @Override
    public List<Integer> getTransactionHistory() {
        return transactionHistory;
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
    private AccountRemoteIface account = null;

    public void jspInit() {
        try {
            account = (AccountRemoteIface) new InitialContext()
                .lookup(&quot;java:global/ExampleEJB-1.0/AccountBean&quot;);
        } catch (NamingException e) {}
    }
%&gt;

&lt;%
    // TODO abc
    String action = request.getParameter(&quot;action&quot;);
    if (action != null &amp;&amp; !action.isEmpty()) {
        int amount = Integer.parseInt(request.getParameter(&quot;amount&quot;));
        if (action.equals(&quot;+&quot;)) {
            account.deposit(amount);
        } else {
            account.withdraw(amount);
        }
    }
%&gt;

&lt;!DOCTYPE html&gt;
&lt;html&gt;
    &lt;head&gt;
        &lt;meta http-equiv=&quot;Content-Type&quot; content=&quot;text/html; charset=UTF-8&quot;&gt;
        &lt;title&gt;EJB Account&lt;/title&gt;
    &lt;/head&gt;
    &lt;body&gt;
        &lt;h1&gt;Account&lt;/h1&gt;

        &lt;p&gt;&lt;strong&gt;Balance: &lt;%= account.getBalance() %&gt;&lt;/strong&gt;&lt;/p&gt;

        &lt;form method=&quot;POST&quot;&gt;
            &lt;p&gt;
                &lt;input type=&quot;number&quot; name=&quot;amount&quot;&gt;
                &lt;button type=&quot;submit&quot; name=&quot;action&quot; value=&quot;+&quot;&gt;Deposit&lt;/button&gt;
                &lt;button type=&quot;submit&quot; name=&quot;action&quot; value=&quot;-&quot;&gt;Withdraw&lt;/button&gt;
            &lt;/p&gt;
        &lt;/form&gt;

        &lt;h2&gt;Transaction history&lt;/h2&gt;
        &lt;ul&gt;
        &lt;% for (int amount : account.getTransactionHistory()) { %&gt;
            &lt;li&gt;&lt;%= amount %&gt;&lt;/li&gt;
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