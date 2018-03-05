<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="JSP: Příklad s použitím beanů"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>JSP</li>
                    <li class="active">Příklad s použitím beanů</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">JSP: Příklad s použitím beanů
                        <span class="pull-right">
                            <a href="${pageContext.request.contextPath}/examples/chouhan-game.jsp" class="btn btn-success">Živá ukázka</a>
                            <a href="${pageContext.request.contextPath}/example/jsp-all.zip" class="btn btn-success">Zdrojové kódy</a>
                        </span>
                    </h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">&lt;%@ page contentType=&quot;text/html&quot; pageEncoding=&quot;UTF-8&quot; %&gt;
&lt;%-- Use a bean and store him in the session, so every user can play own game. --%&gt;
&lt;jsp:useBean id=&quot;game&quot; class=&quot;example.jsp.ChouHanGame&quot; scope=&quot;session&quot;/&gt;
&lt;%-- When form is submitted save parameter &quot;wager&quot; to bean via setWager() method --%&gt;
&lt;jsp:setProperty name=&quot;game&quot; property=&quot;wager&quot;/&gt;

&lt;!DOCTYPE html&gt;

&lt;html&gt;
    &lt;head&gt;
        &lt;meta http-equiv=&quot;Content-Type&quot; content=&quot;text/html; charset=UTF-8&quot;&gt;
        &lt;title&gt;Chō-Han Bakuchi&lt;/title&gt;

        &lt;!-- 'Cause unicode dice characters --&gt;
        &lt;link href=&quot;https://fonts.googleapis.com/css?family=Noto+Sans&quot; rel=&quot;stylesheet&quot;&gt;
        &lt;style&gt;
            body { text-align: center; }
            .dices { font: 80px/1 'Noto Sans'; }
        &lt;/style&gt;
    &lt;/head&gt;

    &lt;body&gt;
        &lt;% if (game.getWager() &lt; 0) { %&gt;

        &lt;% game.shakeCup(); %&gt;

        &lt;p&gt;Place your wager on whether the sum total of numbers showing on the
            two dice will be &quot;Chō&quot; (even) or &quot;Han&quot; (odd).&lt;/p&gt;

        &lt;form method=&quot;POST&quot;&gt;
            &lt;button type=&quot;submit&quot; name=&quot;wager&quot; value=&quot;0&quot;&gt;Chō (even)&lt;/button&gt;
            &lt;button type=&quot;submit&quot; name=&quot;wager&quot; value=&quot;1&quot;&gt;Han (odd)&lt;/button&gt;
        &lt;/form&gt;

        &lt;% } else { %&gt;

        &lt;p&gt;&lt;span class=&quot;dices&quot;&gt;&lt;%= game.getDiceAsString() %&gt;&lt;/span&gt;&lt;/p&gt;
        &lt;p&gt;
            &lt;% if (game.getSuccess()) { %&gt;
            &lt;stong&gt;You won &lt;%= game.getWinCount() %&gt; times in a row!&lt;/stong&gt;
            &lt;% } else { %&gt;
            &lt;string&gt;You lost all your money!&lt;/string&gt;
            &lt;% } %&gt;
        &lt;/p&gt;
        &lt;p&gt;&lt;a href=&quot;chouhan-game.jsp&quot;&gt;Try your luck again.&lt;/a&gt;&lt;/p&gt;

        &lt;% game.reset(); %&gt;

        &lt;% } %&gt;
    &lt;/body&gt;
&lt;/html&gt;</code></pre></div>
                        <h4 class="custom-h4">Bean</h4>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.jsp;

import java.util.Random;

public class ChouHanGame {

    private final Random random = new Random();

    private int[] dice = new int[2];
    private int wager = -1;
    private int winCount = 0;

    public int getWager() {
        return wager;
    }

    public void setWager(String wager) {
        try {
            setWager(Integer.parseInt(wager));
        } catch (NumberFormatException e) {
            setWager(-1);
        }
    }

    public void setWager(int wager) {
        this.wager = wager;

        if (getSuccess()) {
            winCount++;
        } else {
            winCount = 0;
        }
    }

    public boolean getSuccess() {
        return (dice[0] + dice[1]) % 2 == wager;
    }

    public int getWinCount() {
        return winCount;
    }

    public int[] getDice() {
        return dice;
    }

    private String diceString(int value) {
        switch (value) {
            case 1:
                return "\u2680";
            case 2:
                return "\u2681";
            case 3:
                return "\u2682";
            case 4:
                return "\u2683";
            case 5:
                return "\u2684";
            case 6:
                return "\u2685";
        }
        return null;
    }

    public String getDiceAsString() {
        return diceString(dice[0]) + diceString(dice[1]);
    }

    public void shakeCup() {
        dice[0] = random.nextInt(6) + 1;
        dice[1] = random.nextInt(6) + 1;
    }

    public void reset() {
        wager = -1;
    }
}</code></pre></div>
                    </div>
                </div>
            </div>

            <jsp:include page="../components/footer.jsp" />
    </body>
</html>