<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form"%>
<%@ taglib uri = "http://java.sun.com/jsp/jstl/core" prefix = "c" %>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="Spring: Příklad MVC aplikace / Přidat uživatele"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/menu.jsp" />
        <jsp:include page="../components/top-panel.jsp" />
        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li class="active">Spring / Příklad MVC aplikace / Přidat uživatele</li>
                </ol>
            </div><!--/.row-->

            <c:if test="${not empty message}">
                <div class="alert bg-success" role="alert">${message} <a href="#" class="pull-right"><span class="glyphicon glyphicon-remove"></span></a></div>
            </c:if>
            <div class="row">
                <div class="col-md-12">
                    <div class="panel panel-default">
                        <div class="panel-heading">
                            Přidat uživatele
                        </div>
                        <div class="panel-body">
                            <div class="col-md-12">
                                <form:form action="add-user" method="post" modelAttribute="user">
                                    <div class="form-group">
                                        <form:label path="name">Jméno<font color="red"><b>*</b></font></form:label>
                                        <form:input path="name" class="form-control" />
                                    </div>
                                    <div class="form-group">
                                        <form:label path="surname">Příjmení<font color="red"><b>*</b></font></form:label>
                                        <form:input path="surname" class="form-control" />
                                    </div>
                                    <div class="form-group">
                                        <form:label path="username">Uživatelské jméno<font color="red"><b>*</b></font></form:label>
                                        <form:input path="username" class="form-control" />
                                    </div>
                                    <div class="form-group">
                                        <form:label path="password">Heslo<font color="red"><b>*</b></font></form:label>
                                        <form:input type="password" path="password" class="form-control" />
                                    </div>
                                    <div class="form-group">
                                        <form:label path="email">Email<font color="red"><b>*</b></font></form:label>
                                        <form:input type="email" path="email" class="form-control" />
                                    </div>
                                    <div class="form-group">
                                        <form:label path="telephone">Telefon</form:label>
                                        <form:input type="telephone" path="telephone" class="form-control" />
                                    </div>
                                    <div class="form-group">
                                        <button type="submit" class="btn btn-primary">Uložit</button>
                                    </div>
                                </form:form>
                            </div>
                        </div>
                    </div>
                </div>

            </div>
        </div>
        <jsp:include page="../components/footer.jsp" />
    </body>
</html>
