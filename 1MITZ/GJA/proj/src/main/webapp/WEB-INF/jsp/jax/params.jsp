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
            <li>JAX</li>
            <li class="active">URI a práce s parametry</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">JAX</h1>
        </div>
    </div>
    <div class="row no-margin">
        <div class="panel panel-primary">
            <div class="panel-heading">Co je JAX?</div>
            <div class="panel-body">
                Java API pro RESTful Web Services (JAX-RS), je set API sloužící k vývoji REST služeb. JAX-RS jedná se o součást Java EE6 a zjednodušuje vývoj REST webu. Ukázkové implementace odpovídají RESTEasy.
            </div>
        </div>
    </div><!-- Co je JUnit? -->
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Ukázky</div>
            <div class="panel-body">
                <div class="panel-body">
                    <p class="custom-text-block">
                        Zvolení metody na základě vybrané cesty.
                    </p>
                </div>
                <h4>Mappování URI na metodu pomocí @Path</h4>
                <a href="${pageContext.request.contextPath}/jax/examples/books">Ukázka využití cesty</a><br>
                <a href="${pageContext.request.contextPath}/jax/examples/books/comic">Ukázka využití zanořené cesty</a>
                <pre class="custom-code"><code class="java">
    //URI matching with @Path
    @GET
    public Response getBook() {
        return Response.status(200).entity("Metoda getBook byla zavolána.").build();
    }

    @GET
    @Path("/comic")
    public Response getComicBook() {
        return Response.status(200).entity("Metoda getComicBook byla zavolána.").build();
    }
</code></pre>

                <div class="panel-body">
                    <p class="custom-text-block">
                        Ukázka zpracování a využití parametru.
                    </p>
                </div>
                <h4>Využití parametru</h4>
                <a href="${pageContext.request.contextPath}/jax/examples/books/name/Inferno">Ukázka využití parametru</a>
                <pre class="custom-code"><code class="java">
    @GET
    @Path("name/{name}")
    public Response getBookByName(@PathParam("name") String name) {
        return Response.status(200).entity("Metoda getBookByName byla zavolána s parametrem \"" + name + "\"").build();
    }
</code></pre>
                <div class="panel-body">
                    <p class="custom-text-block">
                        Ukázka zpracování a využití parametrů.
                    </p>
                </div>
                <h4>Využití parametrů</h4>
                <a href="${pageContext.request.contextPath}/jax/examples/books/releasedate/22/01/2016">Ukázka využití parametrů</a>
                <pre class="custom-code"><code class="java">
    @GET
    @Path("releasedate/{day}/{month}/{year}")
    public Response getBookReleaseDate(
            @PathParam("day") int day,
            @PathParam("month") int month,
            @PathParam("year") int year) {

        return Response.status(200)
                .entity("Metoda getBookReleaseDate byla zavolána s parametry: " + day + " " + month + " " + year)
                .build();

    }
</code></pre>
                <div class="panel-body">
                    <p class="custom-text-block">
                        JAX-RS umožňuje také zpracování regulárních výrazů. V ukázce regex pro ISBN knihy.
                    </p>
                </div>
                <h4>Využití regulárích výrazů</h4>
                <pre class="custom-code"><code class="java">
    @GET
    @Path("/books/{isbn : ^(?:ISBN(?:-1[03])?:? )?(?=[0-9X]{10}$|(?=(?:[0-9]+[- ]){3})" +
            "[- 0-9X]{13}$|97[89][0-9]{10}$|(?=(?:[0-9]+[- ]){4})[- 0-9]{17}$)" +
            "(?:97[89][- ]?)?[0-9]{1,5}[- ]?[0-9]+[- ]?[0-9]+[- ]?[0-9X]$}")
    public Response getBookByISBN(@PathParam("isbn") String isbn) {

        return Response.status(200)
                .entity("Metoda getBookByISBN byla zavolána s parametrem : " + isbn).build();

    }
</code></pre>
                <div class="panel-body">
                    <p class="custom-text-block">
                        Parametry je možné zadat i pomocí @MatricParam.
                    </p>
                </div>
                <h4>Využití maticových parametrů</h4>
                <a href="${pageContext.request.contextPath}/jax/examples/books/namematrix/Saturnin;author=Jirotka;genre=Satira">Ukázka využití @MatrixParam</a>
                <pre class="custom-code"><code class="java">
    @GET
    @Path("namematrix/{name}")
    public Response getBooksWithParams(@PathParam("name") String name,
                             @MatrixParam("author") String author,
                             @MatrixParam("genre") String genre) {

        return Response
                .status(200)
                .entity("Metoda getBooksWithParams byla zavolána s parametry: jméno: " + genre
                        + ", autor: " + author + ", žánr: " + genre)
                .build();

    }
</code></pre>

                <div class="panel-body">
                    <p class="custom-text-block">
                        Obdobně lze využít @QueryParam. V ukázce využívající @DefaultValue, které přiřazuje hodnotu v případ, že není zadaná.
                    </p>
                </div>
                <h4>Využití query parametrů s @DefaultValue</h4>
                <a href="${pageContext.request.contextPath}/jax/examples/books/query?from=2013&to=2018&authors=Kafka&authors=Baudelaire">Ukázka využití @QueryParam</a>
                <pre class="custom-code"><code class="java">
    @GET
    @Path("/query")
    public Response getAuthorsBooks(
            @DefaultValue("0") @QueryParam("from") int from,
            @DefaultValue("2018")@QueryParam("to") int to,
            @QueryParam("authors") List&lt;String&gt; authors) {

        return Response
                .status(200)
                .entity("Metoda getAuthorsBooks byla zavolána pro: od: " + from + ", do: " + to
                        + ", autoři: " + authors.toString()).build();
    }
</code></pre>


            </div>
        </div>
    </div><!-- Odstíňovaní závislostí -->

    <jsp:include page="../components/footer.jsp"/>
</body>
</html>