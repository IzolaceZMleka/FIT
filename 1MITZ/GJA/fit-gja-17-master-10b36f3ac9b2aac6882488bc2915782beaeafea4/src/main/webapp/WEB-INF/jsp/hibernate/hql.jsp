<%@page contentType="text/html" pageEncoding="UTF-8" %>
<!DOCTYPE html>
<html>
<jsp:include page="../components/header.jsp">
    <jsp:param name="title" value="Hlavní strana"/>
</jsp:include>
<body>
<jsp:include page="../components/top-panel.jsp"/>
<jsp:include page="../components/menu.jsp"/>

<div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">
    <div class="row">
        <ol class="breadcrumb">
            <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
            <li>Hibernate</li>
            <li class="active">HQL</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">Hibernate
            </h1>
        </div>
    </div>
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Ukázka HQL</div>
            <div class="panel-body">
                <div class="panel-body">
                    <p class="custom-text-block">
                        Preferovaný způsob práce s objekty je HQL (oproti SQL). Syntaxe je SQL velice podobná, ale HQL je, na rozdíl od SQL, objektově orientované, takže rozumí věcem jako dědičnost, polymorfismus, apod. Dotaz „from java.lang.Object o“ by tedy vrátil úplně všechny persistentní objekty. Nejjednodušší dotaz vypadá takto: „from Zajezd“ – vrátí všechny instance třídy Zajezd.<br>

                        Pokud se v dotazu chceme odvolávat na „Zajezd“, je potřeba mu přiřadit alias: „from Zajezd as zajezd“ – je zvykem pojmenovávat aliasy stejným slovem jako název třídy, ale podle konvence, kterou pojmenováváme atributy. Použili bychom toho následujícím způsobem: „from Zajezd as zajezd where zajezd.nazev = nazev“<br>

                        HQL podporuje klasické konstrukce, na které jsme zvyklí z SQL, jako jsou: INNER a OUTER JOIN, klauzule WHERE nebo WITH, agregační výrazy jako AVG() nebo COUNT(), subdotazy, atd. Další zajímavostí je QBE – Query by Example. Jedná se o způsob dotazování, kdy vytvoříme nový objekt s vlastnostmi, které hledáme a tento objekt poté předáme do HQL a ten nám vrátí objekty se stejnými vlastnostmi.<br>
                    </p>
                </div>

                <h3>FROM</h3>
                <p class="custom-text-block">
                    FROM se používá při nahrávání objektu do paměti.
                </p>
                <pre class="custom-code"><code class="java">
String hql = "FROM Book";
Query query = session.createQuery(hql);
List results = query.list();
</code></pre>

                <h3>AS</h3>
                <p class="custom-text-block">
                    AS slouží k přiřazení alisu pro lepší manipulaci s query. Například v případě dlouhých názvů.
                </p>
                <pre class="custom-code"><code class="java">
String hql = "FROM Book AS B";
Query query = session.createQuery(hql);
List results = query.list();
</code></pre>
                <p class="custom-text-block">
Nebo lze také zapsat přímo po názvu třídy.
                </p>
                <pre class="custom-code"><code class="java">
String hql = "FROM Book B";
Query query = session.createQuery(hql);
List results = query.list();
</code></pre>

                <h3>SELECT</h3>
                <p class="custom-text-block">
                    Užitím SELECT lze možno zužit počet získáných informací. Zíksáme tak jenom požadovanou část namísto celého objektu.
                </p>
                <pre class="custom-code"><code class="java">
String hql = "SELECT B.author FROM Book B";
Query query = session.createQuery(hql);
List results = query.list();
</code></pre>

                <h3>WHERE</h3>
                <p class="custom-text-block">
                    WHERE slouží ke specifikaci získaného objektu.
                </p>
                <pre class="custom-code"><code class="java">
String hql = "FROM Book B WHERE B.author = "Shakespear"";
Query query = session.createQuery(hql);
List results = query.list();
</code></pre>

                <h3>ORDER BY</h3>
                <p class="custom-text-block">
                    ORDER BY umožňuje seřazení výsledku a to sestupně (DESC) nebo vzestupně (ASC).
                </p>
                <pre class="custom-code"><code class="java">
String hql = "FROM Book B WHERE B.genre = "Sci-fi" ORDER BY B.edition DESC";
Query query = session.createQuery(hql);
List results = query.list();
</code></pre>

                <h3>Užívání parametrů</h3>
                <p class="custom-text-block">
                    Hibernate také podporuje pojmenované parametry v HQL.
                </p>
                <pre class="custom-code"><code class="java">
String hql = "FROM Book B WHERE B.isdn = :book_isdn";
Query query = session.createQuery(hql);
query.setParameter("book_isdn",1234567891011);
List results = query.list();
</code></pre>

                <h3>UPDATE</h3>
                <pre class="custom-code"><code class="java">
String hql = "UPDATE Book set edition = :edition "  +
             "WHERE id = :book_isdn";
Query query = session.createQuery(hql);
query.setParameter("edition", 3);
query.setParameter("book_isdn", 1234567891011);
</code></pre>

                <h3>DELETE</h3>
                <pre class="custom-code"><code class="java">
String hql = "DELETE FROM Book "  +
             "WHERE id = :book_isdn";
Query query = session.createQuery(hql);
query.setParameter("book_isdn", 1234567891011);
</code></pre>

                <h3>INSERT</h3>
                <pre class="custom-code"><code class="java">
String hql = "INSERT INTO Book(author, title, edition)"  +
             "SELECT author, title, edition FROM old_book";
Query query = session.createQuery(hql);
</code></pre>

            </div>
        </div>
    </div>

    <jsp:include page="../components/footer.jsp"/>
</body>
</html>