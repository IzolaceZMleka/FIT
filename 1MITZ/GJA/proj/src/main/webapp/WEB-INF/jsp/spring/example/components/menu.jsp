<%@page contentType="text/html" pageEncoding="UTF-8"%>
<c:set var="contextPath" value="${pageContext.request.contextPath}"/>
<div id="sidebar-collapse" class="col-sm-3 col-lg-2 sidebar">
    <form style="text-align: center; padding: 20px 0px 10px 0px;">
        MVC Aplikace
    </form>
    <ul class="nav menu">
        <li class="active"><a href="${pageContext.request.contextPath}/user/show"><span class="glyphicon glyphicon-dashboard"></span> Správa uživatelů</a></li>
    </ul>
</div><!--/.sidebar-->