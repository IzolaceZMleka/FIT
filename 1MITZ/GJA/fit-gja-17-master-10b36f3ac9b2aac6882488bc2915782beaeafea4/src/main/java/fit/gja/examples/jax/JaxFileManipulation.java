package fit.gja.examples.jax;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import javax.ws.rs.Consumes;
import javax.ws.rs.Produces;
import javax.ws.rs.POST;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.core.Response;
import org.jboss.resteasy.annotations.providers.multipart.MultipartForm;
import javax.ws.rs.core.Response.ResponseBuilder;
import java.util.Scanner;

@Path("/file")
public class JaxFileManipulation {

    @POST
    @Path("/upload")
    @Consumes("multipart/form-data")
    public Response uploadFile(@MultipartForm FileUploadForm form) {

        String fileName = "C:\\EAI\\something.png";

        try {
            writeFile(form.getData(), fileName);
        } catch (IOException e) {

            e.printStackTrace();
        }

        System.out.println("Done");

        return Response.status(200)
                .entity("Metoda uploadFile byla zavolána, soubor je nahrán do : " + fileName).build();

    }

    // save to somewhere
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

}