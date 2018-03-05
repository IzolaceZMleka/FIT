<%@page contentType="text/html" pageEncoding="UTF-8"%>
<c:set var="contextPath" value="${pageContext.request.contextPath}"/>
<div id="sidebar-collapse" class="col-sm-3 col-lg-2 sidebar">
    <form style="text-align: center; padding: 20px 0px 10px 0px;">
        Témata přednášek GJA
    </form>
    <ul class="nav menu">
        <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-dashboard"></span> Úvod</a></li>
        <li class="parent">
            <a href="#sub-menu-maven" data-toggle="collapse">
                <span class="glyphicon glyphicon-list"></span> Maven <span data-toggle="collapse" href="#sub-menu-maven" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <ul class="children collapse" id="sub-menu-maven">
                <li>
                    <a href="${pageContext.request.contextPath}/maven/">
                        <span class="glyphicon glyphicon-share-alt"></span> Základní informace
                    </a>
                </li>
                <li>
                    <a href="${pageContext.request.contextPath}/maven/pom">
                        <span class="glyphicon glyphicon-share-alt"></span> Soubor pom.xml
                    </a>
                </li>
            </ul>
        </li>
        <li class="parent">
            <a data-toggle="collapse" href="#sub-menu-testing">
                <span class="glyphicon glyphicon-list"></span> Testování <span data-toggle="collapse" href="#sub-menu-testing" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <ul class="children collapse" id="sub-menu-testing">
                <li>
                    <a href="${pageContext.request.contextPath}/tests/junit">
                        <span class="glyphicon glyphicon-share-alt"></span> JUnit
                    </a>
                </li>
                <li>
                    <a href="${pageContext.request.contextPath}/tests/integration">
                        <span class="glyphicon glyphicon-share-alt"></span> Integrační testování
                    </a>
                </li>
                <li>
                    <a href="${pageContext.request.contextPath}/tests/acceptable">
                        <span class="glyphicon glyphicon-share-alt"></span> Akceptační testování
                    </a>
                </li>
            </ul>
        </li>
        <li class="parent">
            <a href="#">
                <span class="glyphicon glyphicon-list"></span> Servlety <span data-toggle="collapse" href="#sub-menu-servlets" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <!--
            <ul class="children collapse" id="sub-menu-servlets">
                <li>
                    <a class="" href="#">
                        <span class="glyphicon glyphicon-share-alt"></span> Sub Item 1
                    </a>
                </li>
                <li>
                <a class="" href="#">
                        <span class="glyphicon glyphicon-share-alt"></span> Sub Item 2
                </a>
                </li>
                <li>
                    <a class="" href="#">
                        <span class="glyphicon glyphicon-share-alt"></span> Sub Item 3
                    </a>
                </li>
            </ul>-->
        </li>
        <li class="parent">
            <a data-toggle="collapse" href="#sub-menu-jsp">
                <span class="glyphicon glyphicon-list"></span> JSP <span data-toggle="collapse" href="#sub-menu-jsp" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <ul class="children collapse" id="sub-menu-jsp">
                <li>
                    <a class="" href="${pageContext.request.contextPath}/jsp/index">
                        <span class="glyphicon glyphicon-share-alt"></span> Základní informace
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/jsp/example">
                        <span class="glyphicon glyphicon-share-alt"></span> Příklad s použitím beanů
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/ejb/stateless#sec-jsp">
                        <span class="glyphicon glyphicon-share-alt"></span> Příklad se Stateless EJB
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/ejb/stateful#sec-jsp">
                        <span class="glyphicon glyphicon-share-alt"></span> Příklad se Stateful EJB
                    </a>
                </li>
            </ul>
        </li>
        <li class="parent">
            <a href="#sub-menu-ejb" data-toggle="collapse">
                <span class="glyphicon glyphicon-list"></span> EJB <span data-toggle="collapse" href="#sub-menu-ejb" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <ul class="children collapse" id="sub-menu-ejb">
                <li>
                    <a class="" href="${pageContext.request.contextPath}/ejb/index">
                        <span class="glyphicon glyphicon-share-alt"></span> Základní informace
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/ejb/stateless">
                        <span class="glyphicon glyphicon-share-alt"></span> Stateless
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/ejb/stateful">
                        <span class="glyphicon glyphicon-share-alt"></span> Stateful
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/ejb/messages">
                        <span class="glyphicon glyphicon-share-alt"></span> Message Driven
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/ejb/interceptors">
                        <span class="glyphicon glyphicon-share-alt"></span> Interceptors
                    </a>
                </li>
            </ul>
        </li>
        <li class="parent">
            <a data-toggle="collapse" href="#sub-menu-jsf">
                <span class="glyphicon glyphicon-list"></span> JSF <span data-toggle="collapse" href="#sub-menu-jsf" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <ul class="children collapse" id="sub-menu-jsf">
                <li>
                    <a class="" href="${pageContext.request.contextPath}/faces/index">
                        <span class="glyphicon glyphicon-share-alt"></span> Základní informace
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/faces/form">
                        <span class="glyphicon glyphicon-share-alt"></span> Formulář
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/faces/listener">
                        <span class="glyphicon glyphicon-share-alt"></span> Listener
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/faces/ajax">
                        <span class="glyphicon glyphicon-share-alt"></span> AJAX
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/faces/navigation">
                        <span class="glyphicon glyphicon-share-alt"></span> Navigace
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/faces/custom">
                        <span class="glyphicon glyphicon-share-alt"></span> Vlastní komponenty
                    </a>
                </li>
            </ul>
        </li>
        <li class="parent">
            <a data-toggle="collapse" href="#sub-menu-primefaces">
                <span class="glyphicon glyphicon-list"></span> PrimeFaces <span data-toggle="collapse" href="#sub-menu-primefaces" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <ul class="children collapse" id="sub-menu-primefaces">
                <li>
                    <a class="" href="${pageContext.request.contextPath}/primefaces/index">
                        <span class="glyphicon glyphicon-share-alt"></span> Základní informace
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/primefaces/input">
                        <span class="glyphicon glyphicon-share-alt"></span> Input
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/primefaces/data">
                        <span class="glyphicon glyphicon-share-alt"></span> Data
                    </a>
                </li>
            </ul>

        </li>
        <li class="parent">
            <a data-toggle="collapse" href="#sub-menu-jax">
                <span class="glyphicon glyphicon-list"></span> JAX <span data-toggle="collapse" href="#sub-menu-jax" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <ul class="children collapse" id="sub-menu-jax">
                <li>
                    <a class="" href="${pageContext.request.contextPath}/jax/params">
                        <span class="glyphicon glyphicon-share-alt"></span> JAX-RS URI a práce s parametry
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/jax/upload">
                        <span class="glyphicon glyphicon-share-alt"></span> JAX-RS práce se soubory
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/jax/jaxb">
                        <span class="glyphicon glyphicon-share-alt"></span> JAX-RS JAXB
                    </a>
                </li>
            </ul>
        </li>
        <li class="parent">
            <a href="#">
                <span class="glyphicon glyphicon-list"></span> Java Persistance API <span data-toggle="collapse" href="#sub-menu-jpa" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <ul class="children collapse" id="sub-menu-jpa">
                <li>
                    <a class="" href="${pageContext.request.contextPath}/jpa/intro">
                        <span class="glyphicon glyphicon-share-alt"></span> Úvod do JPA
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/jpa/example">
                        <span class="glyphicon glyphicon-share-alt"></span> Praktický příklad JPA
                    </a>
                </li>
            </ul>
        </li>
        <li class="parent">
            <a data-toggle="collapse" href="#sub-menu-hibernate">
                <span class="glyphicon glyphicon-list"></span> Hibernate <span data-toggle="collapse" href="#sub-menu-hibernate" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <ul class="children collapse" id="sub-menu-hibernate">
                <li>
                    <a class="" href="${pageContext.request.contextPath}/hibernate/hibernate-xml">
                        <span class="glyphicon glyphicon-share-alt"></span> Hibernate a mapování pomocí XML
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/hibernate/hibernate-anotations">
                        <span class="glyphicon glyphicon-share-alt"></span> Hibernate a mapování pomocí anotací
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/hibernate/hql">
                        <span class="glyphicon glyphicon-share-alt"></span> HQL - Hibernate Query Language
                    </a>
                </li>
            </ul>
        </li>
        <li class="parent">
            <a href="#">
                <span class="glyphicon glyphicon-list"></span> RMI a JMS <span data-toggle="collapse" href="#sub-menu-rmijms" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <!--
            <ul class="children collapse" id="sub-menu-rmijms">
                <li>
                    <a class="" href="#">
                        <span class="glyphicon glyphicon-share-alt"></span> Sub Item 1
                    </a>
                </li>
                <li>
                <a class="" href="#">
                        <span class="glyphicon glyphicon-share-alt"></span> Sub Item 2
                </a>
                </li>
                <li>
                    <a class="" href="#">
                        <span class="glyphicon glyphicon-share-alt"></span> Sub Item 3
                    </a>
                </li>
            </ul> -->
        </li>
        <li class="parent">
            <a href="#">
                <span class="glyphicon glyphicon-list"></span> Spring <span data-toggle="collapse" href="#sub-menu-spring" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span>
            </a>
            <ul class="children collapse" id="sub-menu-spring">
                <li>
                    <a class="" href="${pageContext.request.contextPath}/spring/configuration">
                        <span class="glyphicon glyphicon-share-alt"></span> Příprava a konfigurace
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/spring/beans">
                        <span class="glyphicon glyphicon-share-alt"></span> Správa beans
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/spring/injection">
                        <span class="glyphicon glyphicon-share-alt"></span> Dependency injection
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/spring/annotation">
                        <span class="glyphicon glyphicon-share-alt"></span> Anotace
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/spring/events">
                        <span class="glyphicon glyphicon-share-alt"></span> Události
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/spring/jdbc">
                        <span class="glyphicon glyphicon-share-alt"></span> Integrace JDBC
                    </a>
                </li>
                <li>
                    <a class="" href="${pageContext.request.contextPath}/spring/mvc">
                        <span class="glyphicon glyphicon-share-alt"></span> Příklad MVC aplikace
                    </a>
                </li>
            </ul>
        </li>
    </ul>
</div><!--/.sidebar-->