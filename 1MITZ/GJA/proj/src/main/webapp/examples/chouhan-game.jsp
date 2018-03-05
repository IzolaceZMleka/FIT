<%@ page contentType="text/html" pageEncoding="UTF-8" %>
<%-- Use a bean and store him in the session, so every user can play own game. --%>
<jsp:useBean id="game" class="fit.gja.examples.beans.ChouHanGame" scope="session"/>
<%-- When form is submitted save parameter "wager" to bean via setWager() method --%>
<jsp:setProperty name="game" property="wager"/>

<!DOCTYPE html>

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Chō-Han Bakuchi</title>

        <%-- 'Cause unicode dice characters --%>
        <link href="https://fonts.googleapis.com/css?family=Noto+Sans" rel="stylesheet">
        <style>
            body { text-align: center; }
            .dices { font: 80px/1 'Noto Sans'; }
        </style>
    </head>

    <body>
        <% if (game.getWager() < 0) { %>

        <% game.shakeCup(); %>

        <p>Place your wager on whether the sum total of numbers showing on the
            two dice will be "Chō" (even) or "Han" (odd).</p>

        <form method="POST">
            <button type="submit" name="wager" value="0">Chō (even)</button>
            <button type="submit" name="wager" value="1">Han (odd)</button>
        </form>

        <% } else { %>

        <p><span class="dices"><%= game.getDiceAsString() %></span></p>
        <p>
            <% if (game.getSuccess()) { %>
            <stong>You won <%= game.getWinCount() %> times in a row!</stong>
            <% } else { %>
            <string>You lost all your money!</string>
            <% } %>
        </p>
        <p><a href="chouhan-game.jsp">Try your luck again.</a></p>

        <% game.reset(); %>

        <% } %>
    </body>
</html>
