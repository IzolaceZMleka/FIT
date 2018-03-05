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
            <li>JAX-RS práce se soubory</li>
            <li class="active">JAX-RS práce se soubory</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">JAX-RS práce se soubory</h1>
        </div>
    </div>
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Nahrání souboru</div>
			<div class="panel-body">
				<p class="custom-text-block">
					JAX umožňuje také práci se soubory. Jako první se podíváme na nahrání souboru na server s využitím RESTEasy.
				</p>
			</div>
            <div class="panel-body">
                <h5>POJO třída, která mapuje soubor na tuto třídu</h5>
                <pre class="custom-code"><code class="java">
import javax.ws.rs.FormParam;
import org.jboss.resteasy.annotations.providers.multipart.PartType;

public class FileUploadForm {

	public FileUploadForm() {
	}

	private byte[] data;

	public byte[] getData() {
		return data;
	}

	@FormParam("uploadedFile")
	@PartType("application/octet-stream")
	public void setData(byte[] data) {
		this.data = data;
	}

}
</code></pre>
				<div class="panel-body">
					<p class="custom-text-block">
						Dále je třeba vytvořit službu. Ta využívá @POST a přijímá @MultipartForm.
					</p>
				</div>
                <h4>Služba osluhující nahrání souboru na umístění "C:\\EAI"</h4>
                <pre class="custom-code"><code class="java">
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.core.Response;
import org.jboss.resteasy.annotations.providers.multipart.MultipartForm;

@Path("/file")
public class UploadFileService {

	@POST
	@Path("/upload")
	@Consumes("multipart/form-data")
	public Response uploadFile(@MultipartForm FileUploadForm form) {

		String fileName = "C:\\EAI\\test.png";

		try {
			writeFile(form.getData(), fileName);
		} catch (IOException e) {

			e.printStackTrace();
		}

		System.out.println("Done");

		return Response.status(200)
		    .entity("Metoda uploadFile byla zavolána, cesta k souboru je : " + fileName).build();

	}

                    ...
</code></pre>
                <h4>Metoda obstaravájící zápis na dané místo</h4>
                <pre class="custom-code"><code class="java">
                    ...

	private void writeFile(byte[] content, String filename) throws IOException {

		File file = new File(filename);

		if (!file.exists()) {
			file.createNewFile();
		}

		FileOutputStream fop = new FileOutputStream(file);

		fop.write(content);
		fop.flush();
		fop.close();

	}
</code></pre>

                <h4>Jednoduchý formulář pro nahrání souboru</h4>
                <pre class="custom-code"><code class="html">
&lt;html&gt;
&lt;body&gt;
	&lt;h1&gt;JAX-RS Upload Form&lt;/h1&gt;

                    &lt;form action="rest/file/upload" method="post" enctype="multipart/form-data"&gt;

	   &lt;pv
		Select a file : &lt;input type="file" name="uploadedFile" size="50" /&gt;
	   &lt;/p&gt;

	   &lt;input type="submit" value="Upload It" /&gt;
	&lt;/form&gt;

&lt;/body&gt;
&lt;/html&gt;
				</code></pre>

				<a href="${pageContext.request.contextPath}/jax/uploadForm" class="btn btn-success">Formulář pro nahrání souboru</a>

				<div class="panel-heading">Stažení souboru</div>
				<div class="panel-body">
					<p class="custom-text-block">
						Stažení souboru je o něco jednodušší.
					</p>
				</div>

				<h4>Metoda obstaravájící stažení souboru "lenna.png" ze serveru na "URI/jax/examples/file/get".</h4>

				<pre class="custom-code"><code class="java">
					...

    private static final String FILE_PATH = "lenna.png";

    @GET
    @Path("/get")
    @Produces("image/png")
    public Response getFile() {

        ClassLoader classLoader = getClass().getClassLoader();

        File file = new File(classLoader.getResource(FILE_PATH).getFile());

        ResponseBuilder response = Response.ok((Object) file);
        response.header("Content-Disposition",
                "attachment; filename=testimage.png");
        return response.build();

    }
</code></pre>

				<a href="${pageContext.request.contextPath}/jax/examples/file/get" class="btn btn-success">Stažení souboru</a>
            </div>
        </div>
    </div><!-- Odstíňovaní závislostí -->

    <jsp:include page="../components/footer.jsp"/>
</body>
</html>