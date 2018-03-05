<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@ taglib uri = "http://java.sun.com/jsp/jstl/core" prefix = "c" %>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="Spring: Příklad MVC aplikace"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/menu.jsp" />
        <jsp:include page="../components/top-panel.jsp" />
        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li class="active">Spring / Příklad MVC aplikace</li>
                </ol>
            </div><!--/.row-->

            <c:if test="${not empty message}">
                <div class="alert bg-success" role="alert">${message} <a href="#" class="pull-right"><span class="glyphicon glyphicon-remove"></span></a></div>
            </c:if>
            <div class="row">
                <div class="col-md-12">
                    <div class="panel panel-default">
                        <div class="panel-heading">
                            <div class="row">
                                <div class="col-md-8">Seznam uživatelů</div>
                                <div class="col-md-4 text-right">
                                    <a href="${pageContext.request.contextPath}/user/add-user"><button type="button" class="btn btn-primary">Přidat uživatele</button></a>
                                </div>	
                            </div>
                        </div>
                        <div class="panel-body">
                            <div class="table-responsive">
                                <table class="table table-striped table-bordered table-hover">
                                    <thead>
                                        <tr>
                                            <th>
                                                Jméno a Příjmení
                                            </th>
                                            <th>
                                                Uživatelské jméno
                                            </th>
                                            <th>
                                                Email
                                            </th>
                                            <th>
                                                Telefon
                                            </th>
                                            <th>Akce</th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                    <c:if test="${userList.size() != 0}">
                                        <c:forEach var="user" items="${userList}">
                                            <tr>
                                                <td>${user.name} ${user.surname}</td>
                                                <td>${user.username}</td>
                                                <td>${user.email}</td>
                                                <td>${user.telephone}</td>
                                                <td>
                                                    <a href="${pageContext.request.contextPath}/user/remove-user?id=${user.id}">
                                                        <button type="button" class="btn btn-default">
                                                            <span class="glyphicon glyphicon-remove" aria-hidden="true"></span>
                                                        </button>
                                                    </a>
                                                </td>
                                            </tr>
                                        </c:forEach>
                                    </c:if>
                                    </tbody>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>

            </div>
        </div>
        <jsp:include page="../components/footer.jsp" />
    </body>
</html>
