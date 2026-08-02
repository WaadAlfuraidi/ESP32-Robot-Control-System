const buttons=document.querySelectorAll("button[data-command]");
const message=document.getElementById("message");
const lastCommand=document.getElementById("lastCommand");
const connectionText=document.getElementById("connectionText");
const dot=document.getElementById("dot");
const names={F:"للأمام",B:"للخلف",L:"لليسار",R:"لليمين",S:"توقف"};

function loading(active){
  buttons.forEach(b=>b.disabled=active);
  dot.classList.toggle("loading",active);
  dot.classList.remove("error");
  connectionText.textContent=active?"جاري الاتصال بقاعدة البيانات...":"جاهز لإرسال الأوامر";
}

async function sendCommand(command){
  loading(true);
  message.textContent="جاري حفظ الأمر...";
  try{
    const response=await fetch("update_command.php",{
      method:"POST",
      headers:{"Content-Type":"application/x-www-form-urlencoded;charset=UTF-8"},
      body:new URLSearchParams({command})
    });
    let result;
    try{result=await response.json()}
    catch{throw new Error("استجابة السيرفر غير صحيحة. راجعي ملفات PHP وبيانات قاعدة البيانات.")}
    if(!response.ok||!result.success) throw new Error(result.message||"تعذر تحديث الأمر.");
    lastCommand.textContent=`${result.command} — ${names[result.command]||"غير معروف"}`;
    message.textContent="تم حفظ الأمر بنجاح في قاعدة البيانات.";
    connectionText.textContent="تم الاتصال بنجاح";
    dot.classList.remove("loading","error");
  }catch(error){
    dot.classList.remove("loading");dot.classList.add("error");
    connectionText.textContent="تعذر إرسال الأمر";
    message.textContent=error.message;
  }finally{buttons.forEach(b=>b.disabled=false)}
}

buttons.forEach(button=>button.addEventListener("click",()=>sendCommand(button.dataset.command)));
