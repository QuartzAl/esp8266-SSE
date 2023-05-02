import express from "express";
import ViteExpress from "vite-express";

const app = express();

app.get("/data", (req, res) => {
  res.send("50.5,30.5");
});

app.get("/events", async function (req, res) {
  // Set headers for SSE
  res.set({
    "Cache-Control": "no-cache",
    "Content-Type": "text/event-stream",
    Connection: "keep-alive",
  });
  res.flushHeaders();

  // Let's make the stand go spinny spinny lol. osu! intensifies.
  let count = 0;

  while (true) {
    await new Promise((resolve) => setTimeout(resolve, 16));

    // Emit SSE containing coordinates spinning around a circle
    const message = `data: [${
      50.5 + Math.sin((count / 10) * Math.PI) * 0.005
    },${30.5 + Math.cos((count / 10) * Math.PI) * 0.005}]\n\n`;

    res.write(message);
    count++;
  }
});

ViteExpress.listen(app, 3000, () =>
  console.log("Server is listening on port 3000...")
);
