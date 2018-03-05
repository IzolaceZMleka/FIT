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
            <li class="active">Integrační testování</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">Testování: Integrační testování</h1>
        </div>
    </div>
    <div clas="row">
        <div class="panel panel-primary">
            <div class="panel-heading">Na co jsou integrační test?</div>
            <div class="panel-body">
                <p class="custom-text-block">Slouží pro testování více komponent dohromady. Jak už název napovídá, tak se testuje interakce
                komponent,
                jinak řečeno jestli komponenty dokážou spolupracovat.</p>
            </div>
        </div>
    </div>
    <div clas="row">
        <div class="panel panel-info">
            <div class="panel-heading">Příklad</div>
            <div class="panel-body">
                <p class="custom-text-block">Máme z hry Othello, dva baliky <code>board</code> a <code>game</code>. Board reprezutuje hrací desku
                    a balíček
                    game obsahuje pravidla pro hru (povolené taky, hráče,... ).
                    Každá třída z balíčku je otestovná jednotkovými testy. Jelikož balíček <code>game</code> je přímo
                    závislý
                    na balíčku <code>board</code>, tak nutné, aby spolu balíčky i dobře komunikovali.
                    K tomu slouží již zmíněné integrační testy.</p>
                <h4>Příklady kde se použijí integrační testy:</h4>
                <ul>
                    <li>Třída <code>othello2015.board.Board</code> potřebuje při inicializaci objekt implementující
                        rozhraní <code>othello2015.board.Rules</code>. Toto rozhraní implementuje třída <code>othello2015.game.ReversiRules</code>.
                        Je nutné otestovat jestli třídy spolu správně komunikují při vytváření/získávání nových polí.
                        Tento test zajišťuje třída <code>othello2015.ITBoardReversiRulesTest</code>.
                    </li>
                    <li>Třída <code>othello2015.game.ReversiRules</code> pracuje s potomkem abstraktní třídy <code>othello2015.board.AbstractBoardFiled</code>
                        Je nutné otestovat jestli se s potomkem správně pracuje při vytváření nových polí.
                        Tento test zajišťuje třída <code>othello2015.ITReversiRulesTest</code>.
                    </li>
                    <li>Třída <code>othello2015.game.Player</code> pracuje s třídou <code>othello2015.board.Board</code>
                        Je nutné otestovat jestli se správně inicializuje hrací deska (ve středu se vloží dva bílé a dvá
                        černé disky). Tento test zajišťuje třída <code>othello2015.ITPlayerBoardTest</code>.
                    </li>
                    <li>Třída <code>othello2015.game.Game</code> pracuje s třídami <code>othello2015.game.Player</code>, <code>othello2015.board.Board</code>,
                        Je nutné otestovat:
                        <ul>
                            <li>inicialize hrací desky (ve středu se vloží dva bílé a dvá černé disky)</li>
                            <li>zda začíná bílý hráč</li>
                            <li>střídání hráčů</li>
                        </ul>
                        Tyto testy zajišťují třídy <code>othello2015.ITGamePlayerTest</code> a <code>othello2015.ITGamedTest</code>.
                    </li>
                </ul>
            </div>
            <div class="panel-footer">
                Zdrojové kódy ke stažení <a href="${pageContext.request.contextPath}/example/tests-integration.zip">ZDE</a>.
            </div>
        </div>
    </div>
    <div clas="row">
        <div class="panel panel-info">
            <div class="panel-heading">Příklad 1</div>
            <div class="panel-body">
                <p>Máme z hry Othello, dva baliky <code>board</code> a <code>game</code>. Board reprezutuje hrací desku
                    a balíček
                    game obsahuje pravidla pro hru (povolené taky, hráče,... ).
                    Každá třída z balíčku je otestovná jednotkovými testy. Jelikož balíček <code>game</code> je přímo
                    závislý
                    na balíčku <code>board</code>, tak nutné, aby spolu balíčky i dobře komunikovali.
                    K tomu slouží již zmíněné integrační testy.</p>
                <h4>Příklady kde se použijí integrační testy:</h4>
                <ul>
                    <li>Třída <code>othello2015.board.Board</code> potřebuje při inicializaci objekt implementující
                        rozhraní <code>othello2015.board.Rules</code>. Toto rozhraní implementuje třída <code>othello2015.game.ReversiRules</code>.
                        Je nutné otestovat jestli třídy spolu správně komunikují při vytváření/získávání nových polí.
                        Tento test zajišťuje třída <code>othello2015.ITBoardReversiRulesTest</code>.
                    </li>
                    <li>Třída <code>othello2015.game.ReversiRules</code> pracuje s potomkem abstraktní třídy <code>othello2015.board.AbstractBoardFiled</code>
                        Je nutné otestovat jestli se s potomkem správně pracuje při vytváření nových polí.
                        Tento test zajišťuje třída <code>othello2015.ITReversiRulesTest</code>.
                    </li>
                    <li>Třída <code>othello2015.game.Player</code> pracuje s třídou <code>othello2015.board.Board</code>
                        Je nutné otestovat jestli se správně inicializuje hrací deska (ve středu se vloží dva bílé a dvá
                        černé disky). Tento test zajišťuje třída <code>othello2015.ITPlayerBoardTest</code>.
                    </li>
                    <li>Třída <code>othello2015.game.Game</code> pracuje s třídami <code>othello2015.game.Player</code>, <code>othello2015.board.Board</code>,
                        Je nutné otestovat:
                        <ul>
                            <li>inicialize hrací desky (ve středu se vloží dva bílé a dvá černé disky)</li>
                            <li>zda začíná bílý hráč</li>
                            <li>střídání hráčů</li>
                        </ul>
                        Tyto testy zajišťují třídy <code>othello2015.ITGamePlayerTest</code> a <code>othello2015.ITGamedTest</code>.
                    </li>
                </ul>
            </div>
            <div class="panel-footer">
                Zdrojové kódy ke stažení <a href="${pageContext.request.contextPath}/example/tests-integration.zip">ZDE</a>.
            </div>
        </div>
    </div>

    <jsp:include page="components/footer.jsp"/>
</body>
</html>