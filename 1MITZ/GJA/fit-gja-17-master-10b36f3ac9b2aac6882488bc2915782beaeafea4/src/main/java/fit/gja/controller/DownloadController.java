package fit.gja.controller;

import org.springframework.stereotype.Controller;
import org.springframework.util.FileCopyUtils;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.net.URLConnection;
import java.nio.charset.Charset;

@Controller
@RequestMapping(value = "/example")
public class DownloadController {

    @RequestMapping(value = "/{example_name}.{ext}")
    public void downloadFile(@PathVariable("example_name") String exampleName, @PathVariable("ext") String ext,
                             HttpServletResponse response) throws IOException {
        System.out.println(getClass().getClassLoader().getResource("examples/" + exampleName + "." + ext));
        File file = new File(getClass().getClassLoader().getResource("examples/" + exampleName + "." + ext).getFile());
        System.out.println(getClass().getClassLoader().getResource("examples/" + exampleName + "." + ext));

        if (!file.exists()) {
            String errorMessage = "Soubor neexistuje";
            System.err.println(errorMessage);

            OutputStream stream = response.getOutputStream();
            stream.write(errorMessage.getBytes(Charset.forName("UTF-8")));
            stream.close();
            return;
        }

        String mimeType = URLConnection.guessContentTypeFromName(file.getName());
        if (mimeType == null) {
            System.err.println("MimeType nebylo detekováno");
            mimeType = "application/octet-stream";
        }

        response.setContentType(mimeType);
        response.setHeader("Content-Disposition", String.format("inline; filename\"" + file.getName() + "\""));
        response.setContentLength((int) file.length());


        InputStream stream = new BufferedInputStream(new FileInputStream(file));
        FileCopyUtils.copy(stream, response.getOutputStream());
        response.flushBuffer();
    }
}
